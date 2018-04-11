module Divp.Var where

data Var
  = Var {unVar :: Int} -- index within cache
  | Const {unConst :: String} -- constant or unbound variable
  deriving (Eq)
{-
instance Eq Var where
  (Var _ i) == (Var _ j) = i == j
  (Const a) == (Const b) = a == b
  _ == _ = False
-}

isVar :: Var -> Bool
isVar (Var _) = True
isVar _ = False

instance Show Var where
  show (Var i) = "x" ++ show i
  show (Const i) = i

