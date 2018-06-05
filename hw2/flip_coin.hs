flipping :: String -> Int -> [Int]
flipping [] list = [0] -- If string is empty it is done
flipping [x] list
  | x == 'H' = [0] -- Done
  | otherwise = [list + 1, 0] -- need to flip this coin
flipping (x:xs) list
  | x == y = flipping xs (list + 1) -- If x and head of xs is same need to flip more coin 
  | otherwise = [list + 1] ++ flipping xs (list + 1) -- When x and head of xs is different it need to flip more coin and next coin 
    where y = head xs

check :: String -> Bool -- Check if string has other alphabet
check [] = False
check [x] =  x == 'H' || x == 'T'
check (x:xs) = check xs && (x == 'H' || x == 'T')

flipCoin :: String -> [Int]
flipCoin xs
  | xs == [] = []  -- If string is null return empty string
  | check xs == True = flipping xs 0 -- If string is composed of only 'H' and 'T'
  | otherwise = [] -- If string contains alphabet except 'H' and 'T' return empty string


--flipCoin :: String -> String
--flipCoin xs
--  | check xs == True = show (flipping xs 0)
--  | otherwise = show "Invalid Input"
