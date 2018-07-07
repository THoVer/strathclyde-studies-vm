--Exercise 1

countocc :: Eq a => a -> [a] -> Int
countocc x [] = 0
countocc x (y:ys) = if x == y  then 1 + (countocc x ys) else countocc x ys
                  
--Exercise 2

data Nat = Zero | Nxt Nat
   deriving Show

--Exercise 2(a)
myAbs :: Int -> Int
myAbs x = if x < 0 then -1 * x  else x

absNat :: Int -> Nat
absNat x = if x /= 0 then Nxt(absNat(x - (quot x (myAbs (x))))) else Zero

--Exercise 2(b)

mymult::Nat->Nat->Nat
mymult (Zero) x = Zero
mymult x Zero = Zero
mymult (Nxt x) y = myadd (mymult x y) y

myadd:: Nat -> Nat -> Nat
myadd (Zero) x = x
myadd (Nxt x) y = Nxt (myadd x y)

--Exercise 3
--(a)
insert :: Ord b => (a -> b) -> a -> [a] -> [a]
insert f x [] = [x]
insert f x (y:ys) = if f(y) > f(x) then x:y:ys else y:(insert f x ys)

--(b) 
inssort :: Ord b => (a -> b) -> [a] -> [a]
inssort f [] = []
inssort f (x:xs) =  insert f x (inssort f xs)

--(c) Order a list of strings according to length
countoccClosure :: Char -> (String -> Int)
countoccClosure x = (\z -> countocc x z)

sortwords :: Char -> [String] -> [String]
sortwords x [] = []
sortwords x (y:ys) =  insert (countoccClosure x) y (sortwords x ys)



type Position = (Int,Int)
data Step = L | R | U | D
    deriving (Eq)

--Ex 4(a)

move :: Step -> Position -> Position
move s (x,y) | s == L = (x-1, y)
             | s == R = (x+1, y)
             | s == U = (x, y+1)
             | s == D = (x, y-1)

-- move L (3,4) == (2,4)
-- move U (0,0) == (0,1)

--Ex 4(b)

type Route = [Step]

positions :: Route -> Position-> [Position]
positions [] p = [p]
positions (s:ss) p = p:(positions ss (move s p))

--positions [L,R,L,L,U] (0,0) == [(0,0),(-1,0),(0,0),(-1,0),(-2,0),(-2,1)]