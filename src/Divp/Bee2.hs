module Divp.Bee2 where

import Divp.Var
import Divp.MVar
import Divp.Sym
import Divp.Cache
import Divp

import Control.Monad.State (runState)
import Data.List (intercalate)

class Bee2var v where
  bee2var :: v -> String

instance Bee2var Var where
  bee2var (Var i) = "s(" ++ show i ++ ")"
  bee2var (Const s) = s

instance Bee2var MVar where
  bee2var (MVar i) = "s(" ++ show i ++ ")"
  bee2var (MConst s) = s

stmtBee2 :: (v -> String) -> v -> Sym v -> String
stmtBee2 f i = g where
  argsBee2 = intercalate ", " . map f

  g (Add j k) = "qrAdd(" ++ argsBee2 [i,j,k] ++ ", qr);"
  g (Sub j k) = "qrSub(" ++ argsBee2 [i,j,k] ++ ", qr);"
  g (Sqr j) = "qrSqr(" ++ argsBee2 [i,j] ++ ", qr, stack);"
  g (Mul j k) = "qrMul(" ++ argsBee2 [i,j,k] ++ ", qr, stack);"
  g (Inv j) = "qrInv(" ++ argsBee2 [i,j] ++ ", qr, stack);"

dumpBee2 :: Bee2var v => Cache String v -> String
dumpBee2 cache =
  unlines . reverse . foldCache f [] $ cache
  where
    ii "" = ""
    ii i = " // " ++ i
    f lineno (v,(s,i)) ss = (stmtBee2 bee2var v s ++ ii i): ss

runDivp0 :: Bool -> Int -> ([[Var]], CacheVar)
runDivp0 jcoord k = (nPs,cache) where
  a = Const "a"
  b = Const "b"
  x = Const "x"
  y = Const "y"
  divp = if jcoord then divpJ else divpA
  (nPs,cache) = flip runState emptyCache $
    divp qrM a b (x,y) k

realloc :: ([[Var]], CacheVar) -> CacheMVar
realloc (nPs,cache) = mcache' where
  translateC i (x,j) = (x, MConst s) where
    s = "nP(" ++ show i ++ "," ++ show j ++ ")"
  translateP (nP,i) = map (translateC i) $ zip nP [0..]
  translate = concatMap translateP $ zip nPs [0..]
  mcache = reallocCache True translate $ cache

  rename1 :: MVar -> MVar
  rename1 (MConst "1") = MConst "qr->unity"
  rename1 v = v
  Cache size entries = mcache
  entries' = map (\(v,(s,i)) -> (rename1 v,(fmap rename1 s, i))) entries
  mcache' = Cache size entries'

runDivp :: Bool -> Int -> CacheMVar
runDivp jcoord k = mcache' where
  (nPs,cache) = runDivp0 jcoord k
  mcache' = realloc (nPs,cache)

ecDivp :: Bool -> Int -> String
ecDivp jcoord k = wrapEcDivp jcoord k $ runDivp jcoord k

statDivp :: Bool -> Int -> String
statDivp jcoord k = ds where
  mcache = runDivp jcoord k
  ds = unlines
    [ "C: " ++ show (cacheCost mcache)
    , "S: " ++ show (cacheSize mcache)
    ]

dumpDivp :: Bool -> Int -> String
dumpDivp jcoord k = ds where
  mcache = runDivp jcoord k
  pfx = 
    [ "C: " ++ show (cacheCost mcache)
    , "S: " ++ show (cacheSize mcache)
    ]
  ds = unlines pfx ++ dumpCache' mcache

dumpDivp' :: Bool -> Int -> String
dumpDivp' jcoord k = ds where
  (_,cache) = runDivp0 jcoord k
  pfx = 
    [ "C: " ++ show (cacheCost cache)
    , "S: " ++ show (cacheSize cache)
    ]
  ds = unlines pfx ++ dumpCache' cache

wrapEcDivp :: Bool -> Int -> CacheMVar -> String
wrapEcDivp jcoord k mcache = ds' where
  ds = dumpBee2 mcache

  sizeP = if jcoord then 3 else 2
  name = "ecDivp" ++ (if jcoord then "J" else "A") ++
    "_" ++ show k
  pfx = unlines
    [ "#include \"bee2/core/mem.h\""
    , "#include \"bee2/math/ec.h\""
    , ""
    , "// cost: " ++ show (cacheCost mcache)
    , "// size: " ++ show (cacheSize mcache) ++ "*n words"
    , "void " ++ name ++ "(word *nPs, word const *P, ec_o const *ec, void *stack)"
    , "{"
    , "qr_o const *qr = ec->f;"
    , "size_t const n = qr->n;"
    , "word const *a = ec->A;"
    , "word const *b = ec->B;"
    , "word const *x = P;"
    , "word const *y = P+n;"
    , "word *__s = ((word*)stack) + ec->deep;"
    , "#define s(i) (__s + ((i)*n))"
    , "#define nP(i,j) (nPs+" ++ show sizeP ++ "*(i)+j)"
    ]
  sfx = unlines
    [ "#undef s"
    , "#undef nP"
    , "}"
    , ("size_t " ++ name ++ "_deep(size_t n, size_t ec_deep)")
    , "{"
    , ("  return " ++ (show (cacheSize mcache)) ++ "*n + ec_deep;")
    , "}"
    ]

  ds' = unlines [pfx, ds, sfx]
