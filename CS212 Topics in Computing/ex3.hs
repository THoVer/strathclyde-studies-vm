--Ex 1(a)
checkprod::(Int,Int) -> Int ->Bool
checkprod (n, m) i = n > 0 && m > 0 && n * m <= i

--Ex 1(b) 
smallerprod::Int -> [(Int,Int)]
smallerprod i = [ (n, m) | n <- [1..i], m <- [1..n], checkprod (n, m) i]

-- Ex 2(a)
prime :: Int -> Bool
prime y = if (y > 1) then [ x | x <-[2..y - 1], y `mod` x == 0 ] == [] else False
        
-- Ex 2(b)
primes::[Int]
primes = [x | x <- [1..], prime x]

-- Ex 3
myprefix::String->String->Bool
myprefix (x:xs) [] = False
myprefix [] (y:ys) = True
myprefix (x:xs) (y:ys) = x == y && myprefix xs ys

-- Ex 4
removeRep :: String -> String
removeRep [x] = [x]
removeRep (x:y:xs) = if (x == y) then removeRep(y:xs) else x:removeRep(y:xs)

-- Ex 5
vecSqDist :: [Int] -> [Int] -> Int
vecSqDist [] [] = 0
vecSqDist (x:xs) (y:ys) | length(xs) /= length(ys) = -1
                        | True = (x - y) ^ 2 + vecSqDist xs ys



