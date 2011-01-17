#all: topswop_perm_ex topswop_improve_ex topswop_dfs_ex topswop_dfs2_ex
 
all: test_ex perf_ex

test_ex: test.cpp topswop.h
	g++ -O3 test.cpp -o test_ex

perf_ex: perf.cpp topswop.h
	g++ -O3 perf.cpp -o perf_ex #-fopenmp


#topswop_dfs2_ex: dfs2.cpp topswop.h util.h
#	g++ -O3 dfs2.cpp -o dfs2_ex

clean:
	rm -rf *_ex
