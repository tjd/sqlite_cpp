all: topswop_perm_ex topswop_sqlite_test_ex topswop_prev_dfs_ex node_test_ex topswop_hc_ex topswop_grasp_ex topswop_local_ex
#all: topswop_ex topswop_perm_ex topswop_rev_ex topswop_prev_dfs_ex topswop_sqlite_test

node_test_ex: node_test.cpp topswop.h util.h Node.h
	g++ node_test.cpp -o node_test_ex -l soci_sqlite3-gcc-3_0

topswop_sqlite_test_ex: topswop_sqlite_test.cpp topswop.h util.h
	g++ topswop_sqlite_test.cpp -o topswop_sqlite_test_ex -l soci_sqlite3-gcc-3_0

topswop_grasp_ex: grasp.cpp topswop.h util.h
	g++ grasp.cpp -o topswop_grasp_ex -l soci_sqlite3-gcc-3_0

# topswop_ex: topswop.cpp topswop.h util.h
# 	g++ -O3 topswop.cpp -o topswop_ex
# 
topswop_perm_ex: topswop_perm.cpp topswop.h util.h
	g++ -O2 -funroll-loops topswop_perm.cpp -o topswop_perm_ex -l soci_sqlite3-gcc-3_0

topswop_local_ex: topswop_local.cpp topswop.h util.h
	g++ -O3 topswop_local.cpp -o topswop_local_ex -l soci_sqlite3-gcc-3_0

topswop_hc_ex: topswop_hc.cpp topswop.h util.h
	g++ -O3 topswop_hc.cpp -o topswop_hc_ex -l soci_sqlite3-gcc-3_0


topswop_prev_dfs_ex: topswop_prev_dfs.cpp topswop.h util.h
	g++ -O3 topswop_prev_dfs.cpp -o topswop_prev_dfs_ex -l soci_sqlite3-gcc-3_0


clean:
	rm -rf *_ex
