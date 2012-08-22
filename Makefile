all: exe graphs

exe: c/run cpp/run haskell/run java/Run.class

c/run: c/run.c
	gcc -O3 c/run.c -o c/run

cpp/run: cpp/run.cpp
	g++ --std=c++11 -O3 cpp/run.cpp -o cpp/run

haskell/run: haskell/run.hs
	ghc -O2 -with-rtsopts="-K256M" haskell/run.hs

java/Run.class: java/Run.java
	javac java/Run.java

graphs: graph-time.svg graph-memory.svg

graph-time.svg: graph-time.txt render
	./render graph-time | gnuplot

graph-memory.svg: graph-memory.txt render
	./render graph-memory | gnuplot

graph-time.txt: time_depth c/run cpp/run haskell/run java/Run.class javascript/run perl/run php/run python/run ruby/run
	./time_depth time

graph-memory.txt: time_depth c/run cpp/run haskell/run java/Run.class javascript/run perl/run php/run python/run ruby/run
	./time_depth memory
