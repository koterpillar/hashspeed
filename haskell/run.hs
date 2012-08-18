import Control.Monad.State

import Data.Bits
import qualified Data.Map as M

import System.Environment

type TNode = M.Map Char Tree
data Tree = Leaf Char | Node TNode

instance Show Tree where
    show t = showTree t ""

showTree (Leaf c) _ = show c ++ "\n"
showTree (Node n) prefix = "\n" ++ (M.foldlWithKey' showNode "" n)
    where showNode s k v = s
                         ++ prefix
                         ++ (show k)
                         ++ " -> "
                         ++ (showTree v ('\t':prefix))

data MyRandom = MyRandom Int
initialRandom :: MyRandom
initialRandom = MyRandom 1
myrandom :: State MyRandom Int
myrandom = do
    (MyRandom rnd_state) <- get
    let newstate = (rnd_state * 1103515245 + 12345) .&. 0x7FFFFFFF
    put (MyRandom newstate)
    return newstate

items :: [Char]
items = ['a'..'z']

width :: State MyRandom Int
width = do
    rnd <- myrandom
    return $ rnd `mod` 9 + 1

item :: State MyRandom Char
item = do
    rnd <- myrandom
    return $ items !! (rnd `mod` (length items))

generate :: Int -> State MyRandom Tree
generate 0 = do
    itm <- item
    return $ Leaf itm
generate depth =
    let genItem _ = do
        itm <- item
        st <- generate (depth - 1)
        return (itm, st)
    in do
        twidth <- width
        items <- mapM genItem [1..twidth]
        return $ Node $ M.fromList items

countAccum :: Tree -> State (M.Map Char Int) ()
countAccum (Leaf c) = modify $ M.insertWith (+) c 1
countAccum (Node n) = undefined

count :: Tree -> M.Map Char Int
count tree = execState (return $ countAccum tree) initState
    where initState = M.fromList $ map (\i -> (i, 0)) items

main :: IO ()
main = do
    [depthStr] <- getArgs
    let depth = read depthStr :: Int
    let tree = evalState (generate depth) initialRandom
    print tree
    let counts = count tree
    print counts
    return ()
