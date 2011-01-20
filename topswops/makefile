#all: topswop_perm_ex topswop_improve_ex topswop_dfs_ex topswop_dfs2_ex
 
all: test_ex perf_ex ga_ex

test_ex: test.cpp topswop.h
	g++ -O3 test.cpp -o test_ex

perf_ex: perf.cpp topswop.h
	g++ -O3 perf.cpp -o perf_ex #-fopenmp

ga_ex: ga.cpp topswop.h
	g++ -O3 ga.cpp -o ga_ex #-fopenmp

clean:
	rm -rf *_ex
