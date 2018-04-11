module Main where

import Divp.Bee2
import Divp.Test

import System.Environment (getArgs)

main :: IO ()
main = do
  args <- getArgs
  let usage = "Usage: {a|j|A|J} <k=2..>"
  case args of
    ["test"] -> test
    [scoord,sk]
      | read sk >= 2 -> do
          let
            divp = case scoord of
              "a-" -> statDivp False
              "j-" -> statDivp True
              "a" -> dumpDivp' False
              "j" -> dumpDivp' True
              "A" -> ecDivp False
              "J" -> ecDivp True
              _ -> \_ -> usage
            k = read sk
          putStrLn $ divp k
    _ -> putStrLn usage
