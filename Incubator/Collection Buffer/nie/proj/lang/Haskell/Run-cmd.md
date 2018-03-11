1. [Compiling with -threaded and optimizations on]
   + ghc -O2 --make parallel-demo.hs -threaded -rtsopt
   
   + [Run on 2 cores]
   + time ./parallel-demo +RTS -N2

2. ghc -o fac fac.hs