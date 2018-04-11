{-# LANGUAGE BangPatterns #-}
module Divp.Cost where

import Data.Monoid (Monoid(..))

newtype Cost = Cost (Int,Int,Int,Int) -- I M S A
  deriving (Eq, Ord)

instance Show Cost where
  show (Cost (i,m,s,a)) = show i ++ "I+" ++ show m ++ "M+" ++ show s ++ "S+" ++ show a ++ "A"

instance Semigroup Cost where
  (Cost (!i,!m,!s,!a)) <> (Cost (!i',!m',!s',!a')) = Cost (i+i',m+m',s+s',a+a')

instance Monoid Cost where
  mempty = Cost (0,0,0,0)

