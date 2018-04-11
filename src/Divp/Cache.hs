{-# LANGUAGE FlexibleContexts #-}
module Divp.Cache where

import Divp.Sym
import Divp.Var
import Divp.MVar
import Divp.Cost
import Divp.QR

import Control.Monad (foldM)
import Control.Monad.State (runState)
import Control.Monad.State.Class (MonadState, get, put, modify)
import Data.List (intercalate)

-- (size, [expression])
-- expression: (v,(descr, sym)) (v <- sym // descr)
data Cache info v = Cache
  { cacheSize :: Int
  , cacheExprs :: [(v,(Sym v, info))]
  } deriving (Show)

emptyCache :: Cache info v
emptyCache = Cache 0 []

consCache :: (v,(Sym v, info)) -> Cache info v -> Cache info v
consCache e (Cache size exprs) = Cache size (e:exprs)

pushCacheMVar :: (MVar,(Sym MVar, info)) -> Cache info MVar -> Cache info MVar
pushCacheMVar e@(MVar i,si) (Cache size exprs) = Cache (max size (i+1)) (e:exprs)
pushCacheMVar e cache = consCache e cache

newtype LineNo = Ln Int deriving (Eq, Ord, Show)

findr :: (Int -> a -> Bool) -> [a] -> Maybe (Int,a)
findr pred = snd . foldr f (0,Nothing) where
  f r (j,mr) = (j+1,mr') where
    mr' = case mr of
      Just _ -> mr
      Nothing
        | pred j r -> Just (j,r)
        | otherwise -> Nothing

indexCache :: LineNo -> Cache info v -> Maybe (v, (Sym v, info))
indexCache (Ln lineno) =
  fmap snd .
  findr (\i _ -> i == lineno) .
  cacheExprs

lookupCache :: Eq v => v -> Cache info v -> Maybe (LineNo, (Sym v, info))
lookupCache v =
  fmap (\(l,(v,(s,i))) -> (Ln l, (s, i))) .
  findr (\_ vsi -> v == fst vsi) .
  cacheExprs

findCache :: Eq v => Sym v -> Cache info v -> Maybe (LineNo, (v, info))
findCache s =
  fmap (\(l,(v,(s,i))) -> (Ln l, (v, i))) .
  findr (\_ vsi -> s == fst (snd vsi)) .
  cacheExprs

foldCache :: (LineNo -> (v, (Sym v, info)) -> b -> b) -> b -> Cache info v -> b
foldCache f b = snd . foldr g (0,b) . cacheExprs where
  g vsi (i,b) = (i+1, f (Ln i) vsi b)

dumpCache :: Show v => (info -> String) -> Cache info v -> [String]
dumpCache showInfo = foldCache f [] where
  f (Ln lineno) (v,(s,i)) ss =
    (show lineno ++ ": " ++
    show v ++ " = " ++ show s ++
    showInfo i): ss

dumpCache' :: Show v => Cache String v -> String
dumpCache' = unlines . reverse . dumpCache ss where
  ss "" = ""
  ss s = " \t| " ++ s

modifyCacheInfo' :: ((v, (Sym v, info)) -> info) -> Cache info v -> Cache info v
modifyCacheInfo' f (Cache size exprs) = Cache size exprs' where
  exprs' = map (\vsi@(v,(s,i)) -> (v,(s,f vsi))) exprs

cacheCost :: Cache info v -> Cost
cacheCost = mconcat . map (symCost . fst . snd) . cacheExprs

type CacheVar = Cache String Var

type CacheMVar = Cache String MVar

lastUsed :: Eq v => Cache info v -> [(v,(LineNo,v))]
lastUsed = h . foldl f (0,[]) . cacheExprs where
  h (l,u) = map (\(v',(i,v)) -> (v', (Ln $ l-i-1, v))) u

  f :: Eq v => (Int,[(v,(Int,v))]) -> (v, (Sym v, info)) -> (Int,[(v,(Int,v))])
  f (i,u) (v,(s,_)) = (i+1,u') where
    u' = foldl g u $ symVars s
    g u' v' = case lookup v' u' of
      Just k -> u' -- (Var j) is later used at k
      Nothing -> (v',(i,v)):u'

{-
lastUsed' :: Cache info v -> [Maybe v]
lastUsed' s = snd . foldr f (0,[]) $ s where
  u = lastUsed s
  f _ (i,ms) = (i+1,lookup (Var i) u: ms)
-}

rebindVar :: Show info => Cache info Var -> Cache info MVar -> Var -> MVar
rebindVar cache cache' (Const s) = MConst s
rebindVar cache cache' v =
  case lookupCache v cache of
    Nothing -> error $ "Var " ++ show v ++ " not found in cache " ++ show cache
    Just (lineno,(s,i)) ->
      case indexCache lineno cache' of
        Nothing -> error $ "Var " ++ show v ++ " is outside of cache' " ++ show cache' ++ " at line " ++ show lineno
        Just (v',(s',i')) -> v'

rebindSym :: Show info => Cache info Var -> Cache info MVar -> Sym Var -> Sym MVar
rebindSym cache cache' = fmap (rebindVar cache cache')

reallocCache :: Show info => Bool -> [(Var,MVar)]
  -> Cache info Var -> Cache info MVar
reallocCache canReuseMVar translate cache = mcache where
  freeMVars0 = map MVar . iterate (+1) $ 0
  mcache0 = Cache 0 []
  i0 = (0, (freeMVars0, mcache0))
  mcache = snd . snd . foldr alloc i0 . cacheExprs $ cache
  u = lastUsed cache

  -- free :: Show info => LineNo
  --   -> Cache info Var -> Cache info MVar
  --   -> [MVar] -> Var -> [MVar]
  free lineno cache' freeMVars v = case lookup v u of
    -- var unused (output var), can't free
    Nothing -> freeMVars
    Just (lineno', v')
      | lineno<lineno' -> freeMVars -- can't free, (Var j) is used later in (Var k)
      | lineno'==lineno -> (rebindVar cache cache' v) : freeMVars
      | lineno'<lineno -> error $ "Var " ++ show lineno ++ " is used after " ++ show lineno'

  -- alloc :: Show info => (Var,(Sym Var, info))
  --   -> (Int, ([MVar], Cache info MVar))
  --   -> (Int, ([MVar], Cache info MVar))
  alloc (v,(s,info)) (i,(freeMVars,mcache)) = (i+1,(freeMVars',mcache')) where
    sVars = filter isVar . symVarsUnique $ s
    rebind = rebindSym cache mcache
    freeFold freeMVars = foldl (free (Ln i) mcache) freeMVars $ sVars

    (freeMVars', mcache') = case lookup v translate of
      Just mv -> (freeMVars, consCache (mv, (rebind s,info)) mcache)
      Nothing -> case canReuseMVar of
        True -> 
          let
            (mv:freeMVars') = freeFold freeMVars
            mvsi = (mv,(rebind s, info))
            mcache' = pushCacheMVar mvsi mcache
          in (freeMVars', mcache')
        False ->
          let
            (mv:freeMVars') = freeMVars
            freeMVars'' = freeFold freeMVars'
            mvsi = (mv,(rebind s, info))
            mcache' = pushCacheMVar mvsi mcache
          in (freeMVars'', mcache')


newVar :: info -> Sym Var -> Cache info Var -> (Var, Cache info Var)
newVar i x (Cache size exprs) = (v',cache') where
  v' = Var size
  cache' = Cache (size+1) ((v',(x,i)) : exprs)

renameVar :: Eq v => v -> info -> Cache info v -> Cache info v
renameVar v' i' = modifyCacheInfo' f where
  f (v,(s,i))
    | v==v' = i'
    | otherwise = i

renameVar' :: Eq v => v -> String -> Cache String v -> Cache String v
renameVar' v' i' = modifyCacheInfo' f where
  f (v,(s,i))
    | v==v' = i ++ " | " ++ i'
    | otherwise = i



renameM :: MonadState (CacheVar) m => Var -> String -> m ()
renameM (Const _) descr = return ()
renameM (Var i) descr = modify (renameVar' (Var i) descr)

getnameM :: MonadState (CacheVar) m => Var -> m String
getnameM (Const s) = return $ s
getnameM v@(Var i) = do
  cache <- get
  case lookupCache v cache of
    Nothing -> return ""
    Just (l,(s,i)) -> return $ show l ++ ":" ++ i

cacheM :: MonadState (CacheVar) m => Sym Var -> m Var
cacheM s = do
  cache <- get
  case findCache s cache of
    Nothing -> do
      let (v',cache') = newVar "" s cache
      put cache'
      return v'
    Just (lineno, (v, info)) -> return v

-- assignM :: MonadState (CacheVar) m => Sym Var -> m Var
-- assignM x = do

qrM :: MonadState (CacheVar) m => QR m Var
qrM = QR
  (return $ zero_) -- lookup in cache for zero
  (return $ one_) -- qr->unity
  (\x y -> cacheM (Add x y))
  (\x y -> cacheM (Sub x y))
  sqr_ -- (\x -> cacheM (Sqr x))
  mul_ -- (\x y -> cacheM (Mul x y))
  (\x -> cacheM (Inv x))
  renameM
  getnameM
  where
    zero_ = Const "0"
    one_ = Const "1"
    sqr_ :: MonadState (CacheVar) m => Var -> m Var
    sqr_ v
      | v == zero_ = return zero_
      | v == one_ = return one_
      | otherwise = cacheM (Sqr v)
    mul_ :: MonadState (CacheVar) m => Var -> Var -> m Var
    mul_ v v'
      | v == zero_ = return zero_
      | v' == zero_ = return zero_
      | v == one_ = return v'
      | v' == one_ = return v
      | otherwise = cacheM (Mul v v')


