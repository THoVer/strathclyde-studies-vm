import Data.Char

--Ex 1
myrepeat :: Char -> [Char]
myrepeat x = x:myrepeat x

-- Exercise 2 (a)

myisDigit::Char -> Bool
myisDigit x = x >= '0' && x <= '9'

-- Exercise 2 (b)
myisUpper :: Char -> Bool
myisUpper x = x >= 'A' && x <= 'Z'

offset = ord 'A' - ord 'a'

mytoLower::Char -> Char
mytoLower x = if myisUpper x then chr(ord x - offset) else x

--Ex 3
circlesurface :: Float -> Float
circlesurface x = pi * x^2

-- Ex 4
cycleinc::[Integer] -> [Integer]
cycleinc x = head(x) : cycleinc( tail(x) ++ [(head(x) + 1)] )

-- Ex 5
isAlphabet :: Char -> Bool
isAlphabet x = x >= 'A' && x <= 'z'

compLetters :: Char -> Char -> Char
compLetters x y 
    | isAlphabet(x) == False || isAlphabet(y) == False = error "Input should be alphabetical characters"
    | mytoLower(x) <= mytoLower(y) = x
    | mytoLower(x) >= mytoLower(y) = y