all: topswop_ex topswop_perm_ex topswop_rev_ex topswop_prev_dfs_ex

topswop_ex: topswop.cpp topswop.h
	g++ -O3 topswop.cpp -o topswop_ex

topswop_perm_ex: topswop_perm.cpp topswop.h
	g++ -O3 topswop_perm.cpp -o topswop_perm_ex

topswop_rev_ex: topswop_rev.cpp topswop.h
	g++ -O3 topswop_rev.cpp -o topswop_rev_ex

topswop_prev_dfs_ex: topswop_prev_dfs.cpp topswop.h
	g++ -O3 topswop_prev_dfs.cpp -o topswop_prev_dfs_ex


clean_ex:
	rm -rf *_ex