all: exe graphs

exe: cpp/run haskell/run

cpp/run: cpp/run.cpp
	g++ --std=c++11 -O3 cpp/run.cpp -o cpp/run

haskell/run: haskell/run.hs
	ghc -O2 -with-rtsopts="-K64M" haskell/run.hs

graphs: graph-time.svg graph-memory.svg

graph-time.svg: graph-time.txt
	./render graph-time | gnuplot

graph-memory.svg: graph-memory.txt
	./render graph-memory | gnuplot

graph-time.txt: time_depth cpp/run haskell/run perl/run php/run python/run
	./time_depth time

graph-memory.txt: time_depth cpp/run haskell/run perl/run php/run python/run
	./time_depth memory
