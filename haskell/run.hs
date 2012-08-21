import Control.Monad.State

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
initialRandom = MyRandom 3
myrandom :: State MyRandom Int
myrandom = do
    (MyRandom rnd_state) <- get
    let newstate = (rnd_state + 1) `mod` 1001
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

incChar :: Char -> State (M.Map Char Int) ()
incChar c = modify $ M.insertWith (+) c 1

countAccum :: Tree -> State (M.Map Char Int) ()
countAccum (Leaf c) = incChar c
countAccum (Node n) =
    let countNode (k, v) = do
        incChar k
        countAccum v
    in do
        (flip mapM) (M.toList n) countNode
        return ()

count :: Tree -> M.Map Char Int
count tree =
    let initState = M.fromList $ map (\i -> (i, 0)) items
    in (flip execState) initState $ countAccum tree

main :: IO ()
main = do
    [depthStr] <- getArgs
    let depth = read depthStr :: Int
    let tree = evalState (generate depth) initialRandom
    if depth < 4
        then print tree
        else return ()
    let counts = count tree
    print counts
    return ()
