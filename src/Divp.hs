module Divp where

import Divp.QR

import Control.Monad (foldM)

ssub :: String -> String
ssub = map f where
  chars = "ijkmn-+=" ++ ['0'..'9']
  subs = "ᵢⱼₖₘₙ₋₊₌₀₁₂₃₄₅₆₇₈₉"
  table = zip chars subs
  f c = case lookup c table of
    Just c' -> c'
    Nothing -> c

ssup :: String -> String
ssup = map f where
  chars = "ijkmn-+=" ++ ['0'..'9']
  sups = "ⁱʲᵏᵐⁿ⁻⁺⁼⁰¹²³⁴⁵⁶⁷⁸⁹"
  table = zip chars sups
  f c = case lookup c table of
    Just c' -> c'
    Nothing -> c

showSub :: Show a => a -> String
showSub = ssub . show

showXi :: Int -> String
showXi = ('ξ':) . ssub . show
showXi2 :: Int -> String
showXi2 = (++"²") . showXi

{-
xi2_n :: Monad m => QR m a -> a -> Int -> m a
xi2_n qr xi n = do
  xi2 <- sqr qr xi
  tag qr xi2 $ "ξₙ² " ++ show n
  return xi2

xii_n :: Monad m => QR m a -> a -> a -> Int -> m a
xii_n qr xi1' xi1 n = do
  xii <- mul qr xi1' xi1
  tag qr xii $ "ξₙ₋₁ ξₙ₊₁ " ++ show n
  return xii

y2xii_n :: Monad m => QR m a -> a -> a -> a -> Int -> m a
y2xii_n qr y2 xi1' xi1 n = do
  xii <- xii_n qr xi1' xi1 n
  y2xii <- mul qr y2 xii
  tag qr y2xii $ "(2y)² ξₙ₋₁ ξₙ₊₁ " ++ show n
  return y2xii

y4xii_n :: Monad m => QR m a -> a -> a -> a -> Int -> m a
y4xii_n qr y4 xi1' xi1 n = do
  xii <- xii_n qr xi1' xi1 n
  y4xii <- mul qr y4 xii
  tag qr y4xii $ "(2y)⁴ ξₙ₋₁ ξₙ₊₁ " ++ show n
  return y4xii

xi2_xi_n :: Monad m => QR m a -> a -> a -> a -> a -> Int -> m a
xi2_xi_n qr xi2' xi1'2 xi12 xi2 n = do
  xi2'xi12 <- mul qr xi2' xi12
  tag qr xi2'xi12 $ "ξₙ₋₂ ξₙ₊₁² " ++ show n
  xi2xi1'2 <- mul qr xi2 xi1'2
  tag qr xi2xi1'2 $ "ξₙ₊₂ ξₙ₋₁² " ++ show n

  xi2_xi <- sub qr xi2xi1'2 xi2'xi12
  tag qr xi2_xi $ "ξ₂ₙ/ξₙ = ξₙ₊₂ ξₙ₋₁² - ξₙ₋₂ ξₙ₊₁² " ++ show n
  return xi2_xi
-}

xi34 :: Monad m => QR m a -> a -> a -> a -> m (a, a)
xi34 (QR zero one add sub sqr mul inv tag gettag) a b x = do
  x2 <- sqr x
  tag x2 "x² ← x ²"
  x2a <- add x2 a
  tag x2a "x²+a ← x² + a"
  x2a2 <- sqr x2a
  tag x2a2 "[x²+a]² ← x²+a ²"
  _2x2a2 <- add x2a2 x2a2
  _3x2a2 <- add x2a2 _2x2a2
  tag _3x2a2 "3[x²+a]² ← 3 ⊠ [x²+a]²"

  bx <- mul b x
  tag bx "bx ← b * x"
  _2bx <- add bx bx
  _3bx <- add bx _2bx
  tag _3bx "3bx ← 3 ⊠ bx"
  a2 <- sqr a
  tag a2 "a² ← a ²"
  _3bxa2 <- sub _3bx a2
  tag _3bxa2 "3bx-a² ← 3bx - a²"
  _23bxa2 <- add _3bxa2 _3bxa2
  _43bxa2 <- add _23bxa2 _23bxa2
  tag _43bxa2 "4[3bx-a²] ← 4 ⊠ 3bx-a²"
  xi3 <- add _3x2a2 _43bxa2
  tag xi3 "ξ₃ ← 3[x²+a]² + 4[3bx-a²]"

  _2x2 <- add x2 x2
  _4x2 <- add _2x2 _2x2
  _5x2 <- add x2 _4x2
  tag _5x2 "5x² ← 5 ⊠ x²"
  _5x2a <- sub _5x2 a
  tag _5x2a "5x²-a ← 5x² - a"
  bx_5x2a <- mul bx _5x2a
  tag bx_5x2a "bx[5x²-a] ← bx * 5x²-a"
  _2bx_5x2a <- add bx_5x2a bx_5x2a
  _4bx_5x2a <- add _2bx_5x2a _2bx_5x2a
  tag _4bx_5x2a "4bx[5x²-a] ← 4 ⊠ bx[5x²-a]"
  _5x2a' <- add _5x2 a
  tag _5x2a' "5x²+a ← 5x² + a"
  _a2_5x2a' <- mul a2 _5x2a'
  tag _a2_5x2a' "a²[5x²+a] ← a² * 5x²+a"

  x4 <- sqr x2
  tag x4 "x⁴ ← x² ²"
  _2a <- add a a
  _4a <- add _2a _2a
  tag _4a "4a ← 4 ⊠ a"
  x2_5a <- add x2a _4a
  tag x2_5a "x²+5a ← x²+a + 4a"
  x4x2_5a <- mul x4 x2_5a
  tag x4x2_5a "x⁴[x²+5a] ← x⁴ * x²+5a"
  _2b <- add b b
  _4b2 <- sqr _2b
  _8b2 <- add _4b2 _4b2
  tag _8b2 "8b² ← 8 ⊠ b²"
  -- x⁴[x²+5a]+4bx[5x²-a]-a²[5x²+a]-8b² ← x⁴[x²+5a] + 4bx[5x²-a] -(a²[5x²+a] + 8b²)
  x4x2_5a_4bx_5x2a <- add x4x2_5a _4bx_5x2a
  tag x4x2_5a_4bx_5x2a "x⁴[x²+5a]+4bx[5x²-a] ← x⁴[x²+5a] + 4bx[5x²-a]"
  _a2_5x2a'_8b2 <- add _a2_5x2a' _8b2
  tag _a2_5x2a'_8b2 "a²[5x²+a]+8b² ← a²[5x²+a] + 8b²"
  xi4_2 <- sub x4x2_5a_4bx_5x2a _a2_5x2a'_8b2
  xi4 <- add xi4_2 xi4_2
  tag xi4 "ξ₄ ← 2 ⊠ x⁴[x²+5a]+4bx[5x²-a]-a²[5x²+a]-8b²"

  return (xi3, xi4)

xis :: Monad m => QR m a -> Bool -> a -> a -> (a,a) -> Int -> m [a]
xis qr@(QR zero one add sub sqr mul inv tag gettag) forJ a b (x,y) k = do
  _2y <- add y y
  _2y2 <- sqr _2y
  tag _2y2 "(2y)²"
  _2y4 <- sqr _2y2
  tag _2y4 "(2y)⁴"
  xi0 <- zero
  xi1 <- one
  xi2 <- one
  (xi3,xi4) <- xi34 qr a b x
  let
    xi xis' 0 = return $! xis' ++ [xi0]
    xi xis' 1 = return $! xis' ++ [xi1]
    xi xis' 2 = return $! xis' ++ [xi2]
    xi xis' 3 = return $! xis' ++ [xi3]
    xi xis' 4 = return $! xis' ++ [xi4]
    xi xis' n -- xis' = [ξ₀,ξ₁ .. ξₙ₋₁]
      | even n = do
          let
            m = n `div` 2
            [xi_m2',xi_m1',xi_m0,xi_m1,xi_m2] = take 5 . drop (m-2) $ xis'
          xi2_m1' <- sqr xi_m1'
          tag xi2_m1' $ showXi2 (m-1)
          xi2_m1 <- sqr xi_m1
          tag xi2_m1 $ showXi2 (m+1)

          xi_2m <- case forJ of
            True -> do
              xi_m2m1' <- mul xi_m2 xi2_m1'
              tag xi_m2m1' $ showXi (m+2) ++ " " ++ showXi2 (m-1)
              xi_m2'm1 <- mul xi_m2' xi2_m1
              tag xi_m2'm1 $ showXi (m-2) ++ " " ++ showXi2 (m+1)

              t <- sub xi_m2m1' xi_m2'm1
              mul xi_m0 t

            False -> do
              xi_m0m2 <- mul xi_m0 xi_m2
              tag xi_m0m2 $ showXi (m) ++ " " ++ showXi (m+2)
              xi_m2'm0 <- mul xi_m2' xi_m0
              tag xi_m2'm0 $ showXi (m-2) ++ " " ++ showXi (m)

              t <- mul xi_m0m2 xi2_m1'
              u <- mul xi_m2'm0 xi2_m1
              sub t u

          tag xi_2m $ showXi n
          return $ xis' ++ [xi_2m]
      | odd n = do
          let
            m = (n-1) `div` 2 -- n = 2m+1
            [xi_m1',xi_m0,xi_m1,xi_m2] = take 4 . drop (m-1) $ xis'

          xi2_m0 <- sqr xi_m0
          tag xi2_m0 $ showXi2 m
          xi2_m1 <- sqr xi_m1
          tag xi2_m1 $ showXi2 (m+1)

          xi_m0m2 <- mul xi_m0 xi_m2
          tag xi_m0m2 $ showXi (m) ++ " " ++ showXi (m+2)
          xi_m1'm1 <- mul xi_m1' xi_m1
          tag xi_m1'm1 $ showXi (m-1) ++ " " ++ showXi (m+1)

          xi_m0m23 <- mul xi_m0m2 xi2_m0
          tag xi_m0m23 $ showXi m ++ " " ++ showXi (m+2) ++ "   " ++ showXi2 m

          xi_m1'm13 <- mul xi_m1'm1 xi2_m1
          tag xi_m1'm13 $ showXi (m-1) ++ " " ++ showXi (m+1) ++ "   " ++ showXi2 (m+1)

          xi_2m1 <- case m `mod` 2 of
            0 -> do
              t <- mul _2y4 xi_m0m23
              sub t xi_m1'm13
            1 -> do
              t <- mul _2y4 xi_m1'm13
              sub xi_m0m23 t

          tag xi_2m1 $ showXi n
          return $ xis' ++ [xi_2m1]
  foldM xi [] [0..(2*k+1)]

divpJ :: Monad m => QR m a -> a -> a -> (a,a) -> Int -> m [[a]]
divpJ qr@(QR zero one add sub sqr mul inv tag gettag) a b (x,y) k = do
  xis' <- xis qr True a b (x,y) k
  _2y <- add y y
  _2y2 <- sqr _2y
  let
    nP nPs m = do
      let [xi_m2',xi_m1',xi_m0,xi_m1,xi_m2] = take 5 . drop (m-2) $ xis'
      xi2_m1' <- sqr xi_m1'
      tag xi2_m1' $ showXi2 (m-1)
      xi2_m0 <- sqr xi_m0
      tag xi2_m0 $ showXi2 (m)
      xi2_m1 <- sqr xi_m1
      tag xi2_m1 $ showXi2 (m+1)
      xi_m1'm1 <- mul xi_m1' xi_m1
      tag xi_m1'm1 $ showXi (m-1) ++ " " ++ showXi (m+1)

      xxi2_m0 <- mul x xi2_m0
      _2y2xi_m1'm1 <- mul _2y2 xi_m1'm1
      mPx <- sub xxi2_m0 _2y2xi_m1'm1
      tag mPx ("[" ++ show m ++ "]Px")

      xi_m2m1' <- mul xi_m2 xi2_m1'
      tag xi_m2m1' $ showXi (m+2) ++ " " ++ showXi2 (m-1)
      xi_m2'm1 <- mul xi_m2' xi2_m1
      tag xi_m2'm1 $ showXi (m-2) ++ " " ++ showXi2 (m+1)
      t <- sub xi_m2m1' xi_m2'm1
      mPy <- mul y t
      tag mPy ("[" ++ show m ++ "]Py")

      return $ [mPx, mPy, xi_m0]: nPs
  foldM nP [] $ reverse [3,5..(2*k-1)]

montI :: Monad m => QR m a -> [a] -> m [a]
montI qr@(QR zero one add sub sqr mul inv tag gettag) (x:xs) = do
  let
    f (p:ps) x = do
      xp <- mul x p
      return $ xp : p : ps
  -- x:xs = [x1 .. xn]
  -- p:ps = [xn*..*x2*x1, .. x2*x1, x1]
  (p:ps) <- foldM f [x] xs
  -- p' = (xn*..*x2*x1)^-1
  p' <- inv p
  let
    g (p':p's) (x,p) = do
      x' <- mul p' p
      -- tx <- gettag x'
      -- tag x' $ tx ++ " ^-1"
      q' <- mul p' x
      return (q':x':p's)
  foldM g [p'] $ zip (reverse xs) ps

divpA :: Monad m => QR m a -> a -> a -> (a,a) -> Int -> m [[a]]
divpA qr@(QR zero one add sub sqr mul inv tag gettag) a b (x,y) k = do
  xis' <- xis qr False a b (x,y) k
  _2y <- add y y
  _2y2 <- sqr _2y
  let
    nP nPs m = do
      let [xi_m2',xi_m1',xi_m0,xi_m1,xi_m2] = take 5 . drop (m-2) $ xis'
      xi2_m1' <- sqr xi_m1'
      tag xi2_m1' $ showXi2 (m-1)
      xi2_m0 <- sqr xi_m0
      tag xi2_m0 $ showXi2 (m)
      xi2_m1 <- sqr xi_m1
      tag xi2_m1 $ showXi2 (m+1)
      xi_m1'm1 <- mul xi_m1' xi_m1
      tag xi_m1'm1 $ showXi (m-1) ++ " " ++ showXi (m+1)

      mPx <- mul _2y2 xi_m1'm1
      tag mPx ("[" ++ show m ++ "]Pjx")

      xi_m0m2 <- mul xi_m0 xi_m2
      tag xi_m0m2 $ showXi (m) ++ " " ++ showXi (m+2)
      xi_m2'm0 <- mul xi_m2' xi_m0
      tag xi_m2'm0 $ showXi (m-2) ++ " " ++ showXi (m)

      t <- mul xi_m0m2 xi2_m1'
      u <- mul xi_m2'm0 xi2_m1
      xi_2m <- sub t u

      mPy <- mul y xi_2m
      tag mPy ("[" ++ show m ++ "]Pjy")

      return $ (mPx, mPy, xi2_m0): nPs
  nPjs <- foldM nP [] [3,5..(2*k-1)]

  let zs = map (\(x,y,z) -> z) nPjs
  zs' <- montI qr zs
  let nPjs' = zipWith (\(x,y,z) (z',i) -> ((x,y,z'),i)) nPjs (zip zs' (reverse [3,5..(2*k-1)]))
  let nPa nPas ((xx,yy,z'),i) = do
        z'2 <- sqr z'
        xa <- mul xx z'
        x' <- sub x xa
        tag x' $ "[" ++ show i ++ "]Px"
        ya <- mul yy z'2
        tag ya $ "[" ++ show i ++ "]Py"
        return $ [x',ya] : nPas
  foldM nPa [] nPjs'
