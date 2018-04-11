{-# LANGUAGE DeriveFunctor #-}
module Divp.Sym where

import Divp.Cost

import Data.List (nub)

data Sym v
  = Add v v
  | Sub v v
  | Sqr v
  | Mul v v
  | Inv v
  deriving (Functor)

-- Eq (Sym Var)
instance Eq v => Eq (Sym v) where
  -- (Sym s) == (Sym s') = s == s'
  (Add i j) == (Add i' j') = ((i == i') && (j == j')) || ((i == j') && (j == i'))
  (Sub i j) == (Sub i' j') = (i == i') && (j == j')
  (Sqr i) == (Sqr i') = (i == i')
  (Mul i j) == (Mul i' j') = ((i == i') && (j == j')) || ((i == j') && (j == i'))
  (Inv i) == (Inv i') = (i == i')
  _ == _ = False

instance Show v => Show (Sym v) where
  -- show (Sym s) = s
  show (Add j k) = show j ++ " + " ++ show k
  show (Sub j k) = show j ++ " - " ++ show k
  show (Sqr j) = show j ++ " ²"
  show (Mul j k) = show j ++ " * " ++ show k
  show (Inv j) = show j ++ " ⁻¹"

symCost :: Sym v -> Cost
symCost (Add _ _) = Cost (0,0,0,1)
symCost (Sub _ _) = Cost (0,0,0,1)
symCost (Sqr _) = Cost (0,0,1,0)
symCost (Mul _ _) = Cost (0,1,0,0)
symCost (Inv _) = Cost (1,0,0,0)

symVars :: Sym v -> [v]
symVars = f where
  f (Add i j) = [i,j]
  f (Sub i j) = [i,j]
  f (Sqr i) = [i]
  f (Mul i j) = [i,j]
  f (Inv i) = [i]
symVarsUnique :: Eq v => Sym v -> [v]
symVarsUnique = nub . symVars

