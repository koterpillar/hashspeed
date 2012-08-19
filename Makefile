all: exe

exe: cpp/run haskell/run

cpp/run: cpp/run.cpp
	g++ --std=c++11 -O3 cpp/run.cpp -o cpp/run

haskell/run: haskell/run.hs
	ghc -O2 -with-rtsopts="-K64M" haskell/run.hs
