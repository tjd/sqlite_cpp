# best_log.py
import sqlite3, os, time

logfile = 'best.log'

def str_to_perm(line):
    return [int(s) for s in line.split(', ')]

def perm_score(perm):
    count = 0
    while perm[0] != 1:
        f = perm[0]
        top = perm[:f]
        top.reverse()
        perm = top + perm[f:]
        count += 1
    return count

def append_best_db_to_best_log():
    f = open(logfile, 'a')
    conn = sqlite3.connect('best_so_far.db')
    c = conn.cursor()
    c.execute('SELECT perm FROM topswops')
    for r in c:
        perm = str_to_perm(r[0])
        n = max(perm)
        score = perm_score(perm)
        print 'n = %s, %s\n%s' % (n, score, perm)
        f.write(r[0] + '\n')    

def db_test():
    conn = sqlite3.connect('best_so_far.db')
    c = conn.cursor()
    c.execute('SELECT perm FROM topswops')
    for r in c:
        perm = str_to_perm(r[0])
        n = max(perm)
        score = perm_score(perm)
        print 'n = %s, %s\n%s' % (n, score, perm)

def main():
    st = os.stat(logfile)
    f = open(logfile, 'r')
    best = {}
    count = 0
    for line in f:
        count += 1
        perm = str_to_perm(line)
        n = max(perm)
        score = perm_score(perm)
        if n not in best:
            best[n] = perm
        elif score > perm_score(best[n]):
            best[n] = perm
    lst = [(n, best[n], perm_score(best[n])) for n in best]
    lst.sort()
    for n, p, score in lst:
        print 'n = %s, %s\n%s' % (n, score, p)
    total_score = sum(score for n, perm, score in lst)
    print '   Total score = %s' % total_score
    print '   "%s" last modified: %s' % (logfile,
                                         time.asctime(time.localtime(st[-2])))
    print '   %s lines' % count

if __name__ == '__main__':
    main()
    #db_test()
    #append_best_db_to_best_log()
