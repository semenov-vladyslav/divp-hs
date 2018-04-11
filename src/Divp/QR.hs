module Divp.QR where

data QR m v = QR
  { zero :: m v
  , one :: m v
  , add :: v -> v -> m v
  , sub :: v -> v -> m v
  , sqr :: v -> m v
  , mul :: v -> v -> m v
  , inv :: v -> m v
  , tag :: v -> String -> m ()
  , gettag :: v -> m String
  -- , (:=) :: v -> m v -> m v
  }

newtype Identity a = Id { unId :: a }

qr :: Integer -> QR Identity Integer
qr p = QR (Id $ 0) (Id $ 1)
  (\x y -> Id $ (x + y) `mod` p)
  (\x y -> Id $ (x - y) `mod` p)
  (\x -> Id $ (x * x) `mod` p)
  (\x y -> Id $ (x * y) `mod` p)
  (\x -> Id $ (x ^ (p-2)) `mod` p)
  (\x s -> Id $ ())
  (\x -> Id $ "")

{-
p = 2^256 - 189
f = qr p
a = p-3
b = 0x77CE6C1515F3A8EDD2C13AABE4D8FBBE4CF55069978B9253B22E7D6BD69C03F1
x = 0
y = 0x6BF7FC3CFB16D69F5CE4C9A351D6835D78913966C408F6521E29CF1804516A93
-}
