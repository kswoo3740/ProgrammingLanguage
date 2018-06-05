primes = filterPrime [2..] 
  where filterPrime (p:xs) = p : filterPrime[x | x <- xs, x `mod` p /= 0]

findingPrimes n size
      | n < 1 = take size (filter ( > 1) primes)  --If n is smaller than 0 return prime number from 2
      | size < 1 = [] -- If size is smaller than 1 return empty list
      | otherwise = take size (filter ( > n) primes) -- Return size primenumber bigger than n 
