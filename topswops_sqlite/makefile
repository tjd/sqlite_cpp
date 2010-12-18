all: topswop_perm_ex topswop_sqlite_test_ex
#all: topswop_ex topswop_perm_ex topswop_rev_ex topswop_prev_dfs_ex topswop_sqlite_test

topswop_sqlite_test_ex: topswop_sqlite_test.cpp topswop.h util.h
	g++ topswop_sqlite_test.cpp -o topswop_sqlite_test_ex -l soci_sqlite3-gcc-3_0

# topswop_ex: topswop.cpp topswop.h util.h
# 	g++ -O3 topswop.cpp -o topswop_ex
# 
topswop_perm_ex: topswop_perm.cpp topswop.h util.h
	g++ -O3 topswop_perm.cpp -o topswop_perm_ex -l soci_sqlite3-gcc-3_0
# 
# topswop_rev_ex: topswop_rev.cpp topswop.h util.h
# 	g++ -O3 topswop_rev.cpp -o topswop_rev_ex
# 
# topswop_prev_dfs_ex: topswop_prev_dfs.cpp topswop.h util.h
# 	g++ -O3 topswop_prev_dfs.cpp -o topswop_prev_dfs_ex
# 
# 
clean:
	rm -rf *_ex
