all: topswop_perm_ex topswop_improve_ex topswop_dfs_ex topswop_dfs2_ex
 
topswop_perm_ex: perm.cpp topswop.h util.h
	g++ -O2 -funroll-loops perm.cpp -o perm_ex

topswop_dfs2_ex: dfs2.cpp topswop.h util.h
	g++ -O3 dfs2.cpp -o dfs2_ex

topswop_improve_ex: improve_current.cpp topswop.h util.h
	g++ -O3 improve_current.cpp -o improve_ex

clean:
	rm -rf *_ex
