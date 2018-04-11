module Divp.MVar where

data MVar -- mutable var
  = MVar {unMVar :: Int} -- index within stack
  | MConst {unMConst :: String} -- constant or unbound variable
  -- MVars can't be compared!!!
  -- deriving (Eq)

instance Show MVar where
  show (MVar i) = "t" ++ show i
  show (MConst i) = i

