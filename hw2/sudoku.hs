import Data.Maybe
import Data.List

seperateLine :: Int -> [a] -> [[a]] -- Seperate lines for calculate or print
seperateLine _ [] = []
seperateLine n xs = take n xs : (seperateLine n (drop n xs))

mapMaybe' _ [] _ = [] -- Customize mapMaybe function for 3 parameter
mapMaybe' f (x:xs) y = 
  let rs = mapMaybe' f xs y in
  case f x y of
    Nothing -> rs
    Just r -> r:rs

solve :: [Int] -> [Int] -> Maybe [Int] -- Solve sudoku
solve sudoku blk
  | isSolved sudoku = Just sudoku
  | otherwise = do
    index <- elemIndex 0 sudoku
    let jigsaw = [nextCheck sudoku index i | i <- [1..9], 
                  solveRow (nextCheck sudoku index i) index, 
                  solveColumn (nextCheck sudoku index i) index,
                  solveJigsaw (nextCheck sudoku index i) (blk !! index)]
    listToMaybe (mapMaybe' solve jigsaw blk)
  where nextCheck sudoku index i = take index sudoku ++ [i] ++ drop (index + 1) sudoku -- Change value if find answer
        solveRow sudoku index = (length (getRow sudoku index)) == (length (nub ( getRow sudoku index))) -- solve~ function find the index which is not solved yet
        solveColumn sudoku index = (length (getColumn sudoku index)) == (length (nub (getColumn sudoku index)))
        solveJigsaw sudoku index = (length (getJigsaw sudoku index)) == (length (nub ( getJigsaw sudoku index)))
        getRow sudoku index = filter (/= 0) ((seperateLine 9 sudoku) !! (quot index 9)) -- get~ function seperate line to solve sudoku
        getColumn sudoku index = filter (/= 0) ((transpose (seperateLine 9 sudoku)) !! (mod index 9))
        getJigsaw sudoku index = filter (/= 0) [sudoku !! i | i <- (elemIndices (index) blk)]
    

isSolved :: [Int] -> Bool -- Check is there any 0 left, if 0 exist it is not solved yet
isSolved = not . any (==0)

printSudoku :: [Int] -> String
printSudoku sudoku = intercalate "\n" ( map (intercalate " " . map show) (seperateLine 9 sudoku)) -- Seperate list to 9 lines to print result

squigglySudoku :: [Int]->[Int]->IO() -- Call solve fucntion and print the result
squigglySudoku val blk = do
  putStrLn (printSudoku (fromMaybe [] (solve val blk)))
