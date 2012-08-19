all: exe graphs

exe: cpp/run haskell/run java/Run.class

cpp/run: cpp/run.cpp
	g++ --std=c++11 -O3 cpp/run.cpp -o cpp/run

haskell/run: haskell/run.hs
	ghc -O2 -with-rtsopts="-K256M" haskell/run.hs

java/Run.class: java/Run.java
	javac java/Run.java

graphs: graph-time.svg graph-memory.svg

graph-time.svg: graph-time.txt
	./render graph-time | gnuplot

graph-memory.svg: graph-memory.txt
	./render graph-memory | gnuplot

graph-time.txt: time_depth cpp/run haskell/run java/Run.class perl/run php/run python/run ruby/run
	./time_depth time

graph-memory.txt: time_depth cpp/run haskell/run java/Run.class perl/run php/run python/run ruby/run
	./time_depth memory
