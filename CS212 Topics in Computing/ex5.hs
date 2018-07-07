import System.Random

--Exercise 1

cutoffat :: Eq a => a -> [a] -> [a]
cutoffat y [] = []
cutoffat y (x:xs) | x == y = [x]
                  | otherwise = x:(cutoffat y xs)

--Exercise 2

gencollatzdist :: (Int -> Int) -> Int -> Int
gencollatzdist f x | x == 1 = 0
                   | otherwise = 1 + (gencollatzdist f (f x))

collatz::Int -> Int
collatz n 
        | mod n 2 == 0 = div n 2
        | otherwise = (3 * n + 1) 

-- gencollatzdist (1+) (-4) == 5
-- gencollatzdist (collatz) 1 == 0

--Exercise 3(a)

twoSqrCompare:: Float -> Float
twoSqrCompare x = abs(2 - x * x)

genFloatTwo::[Float]
genFloatTwo = randomRs (0,2) (mkStdGen 42)

approxsqrtTwo :: Int -> Float
approxsqrtTwo n = (fromIntegral(length [x | x <- (take n genFloatTwo), x * x <= 2])) / fromIntegral(n) * 2

-- Exercise 3(b)
genFloat::Float -> [Float]
genFloat d = randomRs (0, d) (mkStdGen 57)


approxsqrt :: Float -> Int -> Float
approxsqrt d n = (fromIntegral(length [x | x <- (take n (genFloat d)), x * x <= d])) / fromIntegral(n) * d

--Exercise 4

countocc :: Char -> String -> Int
countocc c xs = foldr (chHelp c) 0 xs

chHelp :: Char -> Char -> Int -> Int
chHelp c x y | c == x = y + 1
             | otherwise = y

--Exercise 5

data BTree a = Leaf a | Node a (BTree a) (BTree a)
     deriving (Show,Eq)

testtree1 = Node 5 (Node 4 (Leaf 1) (Leaf 2)) (Node 3 (Leaf 3) (Leaf 4))
testtree2 = Node 3 (Leaf 76) (Node 3 (Leaf 8) (Leaf 9))

--(a)
treesum :: Num a => BTree a -> a 
treesum (Leaf x) = x;
treesum (Node n x y) = n + (treesum x) + (treesum y);

--(b)
paths :: BTree a -> [[a]] 
paths (Leaf x) = [[x]]
paths (Node n xn yn) =  [n:x | x <-(paths xn)] ++ [n:y | y <-(paths yn)]