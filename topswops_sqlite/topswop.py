# topswop.py

import random
import os
import sys
import sqlite3
import re

n_vals = [  2,  3,  5,  7, 11, 
           13, 17, 19, 23, 29, 
           31, 37, 41, 43, 47, 
           53, 59, 61, 67, 71,
           73, 79, 83, 89, 97];

# def make_file_name(n):
#     return 'best_so_far/topswop_num_%s.txt' % n
# 
# def make_all_num_files():
#     print 'Are you you want to do this?'
#     print 'All existing number files will be deleted!'
#     r = random.randint(100, 1000)
#     ans = raw_input('Enter %s to go ahead: ' % r)
#     if ans != str(r):
#         print 'Wrong number: nothing done'
#         return
#     
#     for n in n_vals:
#         f = open(make_file_name(n), 'w')
#         f.write('0\n')
#         f.write(', '.join(str(i) for i in range(1, n+1)))
# 
def perm_score(perm):
    count = 0
    while perm[0] != 1:
        f = perm[0]
        top = perm[:f]
        top.reverse()
        perm = top + perm[f:]
        count += 1
    return count

def make_test_db():
    conn = sqlite3.connect('test.db')
    c = conn.cursor()
    c.execute("CREATE TABLE topswops(n INTEGER PRIMARY KEY, score INTEGER, perm TEXT, date TEXT)")
    for n in range(2, 98):
        sql = "insert into topswops(n, score, perm, date) values(%s, %s, '%s', date('now'))" % (n, 0, ', '.join(str(i) for i in range(1, n+1)))
        print sql
        c.execute(sql)
    conn.commit()
    conn.close()

# def add_no_primes():
#     """ One-time function to add non-prime rows to DB.
#     """
#     conn = sqlite3.connect('best_so_far.db')
#     c = conn.cursor()
#     for n in range(4, 97):
#         if n not in n_vals:
#             c.execute("insert into topswops(n, score, perm, date) values(%s, %s, '%s', date('now'))" % (n, 0, ', '.join(str(i) for i in range(1, n+1))))
#     conn.commit()
#     conn.close()
#     
# def transfer_file_to_db():
#     """ One-time function for populating sqlite db with current best
#     scores.
#      "INSERT OR REPLACE INTO topswops(n, score, perm, date) VALUES(" 
#     """
#     #os.chdir('best_so_far')
#     conn = sqlite3.connect('best_so_far.db')
#     c = conn.cursor()
#     c.execute("CREATE TABLE topswops(n INTEGER PRIMARY KEY, score INTEGER, perm TEXT, date TEXT)")
#     for n in n_vals:
#         print 'n = %s' % n
#         f = open('topswop_num_%s.txt' % n, 'r')
#         score = int(f.next().strip())
#         #print 'score = %s' % score
#         s = f.next().strip()
#         #print 's = %s' % s
#         perm = [int(i) for i in s.split(', ')]
#         #print 'perm = %s' % perm
#         sql = "insert into topswops(n, score, perm, date) values(%s, %s, '%s', date('now'))" % (n, score, s)
#         print sql
#         c.execute(sql)
#         print
#     conn.commit()
#     conn.close()
#     check_db_okay()
# 
def get_db_row(n, dbname = 'best_so_far.db'):
    """ Retrieve the best score so far for n.
    """
    conn = sqlite3.connect(dbname)
    c = conn.cursor()
    c.execute('select * from topswops where n = %s' % n)
    return c.fetchone()

def perm_str_to_list(s):
    return [int(i) for i in s.split(', ')]

def is_perm(lst):
    cpy = lst[:]
    cpy.sort()
    return cpy == range(1, len(cpy) + 1)

def check_db_okay(dbname = 'best_so_far.db'):
    print 'Checking "%s" ...' % dbname,
    conn = sqlite3.connect(dbname)
    c = conn.cursor()
    c.execute('select * from topswops')
    error_count = 0
    for row in c:
        n, score, perm, date = row
        if n < 2:
            print 'Error: n = %s is too small (n >= 2)' % n
            error_count += 1
        if score < 0:
            print 'Error: score = %s is too small (score >= 0)' % score
            error_count += 1
        lst = perm_str_to_list(perm)
        if not is_perm(lst):
            print 'Error: not a perm: %s' % perm
            error_count += 1
        if score != perm_score(lst):
            print 'Error: incorrect score for n = %s' % n
            error_count += 1

    if error_count == 0:
        print 'done (no errors)'
        return True
    elif error_count == 1:
        print 'done (1 error)'
        return False
    else:
        print 'done (%s errors)' % error_count
        return False
        

#def get_num_file(n):
#     assert n in n_vals
#     f = open(make_file_name(n), 'r')
#     score = int(f.next())
#     perm = [int(i) for i in f.next().split(', ')]
#     assert score == perm_score(perm), '%s\nscore = %s, perm_score = %s' % (perm, score, perm_score(perm))
#     return score, perm
# 
def gen_num_report(dbname = 'best_so_far.db'):
    """ Assumes DB has a table called topswops with this schema:

    CREATE TABLE IF NOT EXISTS topswops(n INTEGER PRIMARY KEY,
                                        score INTEGER,
                                        perm TEXT,
                                        date TEXT)
    """
    if not check_db_okay():
        print 'Report not generated: DB has errors'
        return
    conn = sqlite3.connect(dbname)
    c = conn.cursor()
    c.execute('select * from topswops')
    
    total_score = 0
    submit = []
    for row in c:
        n, score, perm = row[0], row[1], row[2]
        if n in n_vals:
            total_score += score
            submit.append(perm)
    print
    print 'Submit the following for a score of %s: ' % total_score
    print ';\n\n'.join(submit)


def list_current_best_scores(dbname = 'best_so_far.db'):
    conn = sqlite3.connect(dbname)
    c = conn.cursor()
    c.execute('select * from topswops')

    print ' n  score'
    print '========='
    total_score = 0
    for row in c:
        n, score, perm = row[0], row[1], row[2]
        print '%2s  %4s' % (n, score)
        total_score += score
    print '---------'    
    print '    %s' % total_score


if __name__ == '__main__':
    params = sys.argv[1:]
    if params == []:
        gen_num_report()
    elif params[0] == 'report':
        gen_num_report()
    elif params[0] == 'scores':
        list_current_best_scores()
    elif params[0] == 'check':
        check_db_okay()
    elif params[0] == 'maketestdb':
        make_test_db()
#     elif params[0] == 'addnoprimes':
#         add_no_primes()
# 
#    elif params[0] == 'transfer':
#        transfer_file_to_db()
