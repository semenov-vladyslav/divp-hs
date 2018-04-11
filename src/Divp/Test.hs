module Divp.Test where

import Divp.QR
import Divp.Var
import Divp.MVar
import Divp.Cache

import Control.Monad.State (runState)

test :: IO ()
test = do
  let ([z1,z2],s) = runState (tt qrM (Const "a") (Const "b")) emptyCache
  let translate = [(z1,MConst "z1"),(z2,MConst "z2")]
  putStrLn (dumpCache' $ s)
  putStrLn ""
  putStrLn ("z1 z2 = " ++ show [z1,z2])
  putStrLn ""
  putStrLn (dumpCache' . reallocCache True translate $ s)
  putStrLn ""
  putStrLn (dumpCache' . reallocCache False translate $ s)
  putStrLn ""
  putStrLn $ show $ lastUsed s

  where
    tt :: Monad m => QR m a -> a -> a -> m [a]
    tt (QR zero one add sub sqr mul inv tag gettag) a b = do
      x <- add a b
      y <- mul a b
      t <- add x x
      u <- sqr t
      tag u "u"
      z1 <- sqr y
      v <- mul z1 u
      z2 <- sub y v
      return [z1, z2]
    {-
[ Sub (Var 1) (Var 3) -- z2
, Sqr (Var 1) -- z1
, Sqr (Var 2) -- u
, Add (Var 0) (Var 0) -- t
, Mul (Const "a") (Const "b") -- y
, Add (Const "a") (Const "b") -- x
]
    -}

