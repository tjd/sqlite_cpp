# best_log.py
import sqlite3, os, time

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

def remake_master_log():
    """ Combine all .log files (other than master.log) into master.log.
    """
    print 'Re-making log/master.log ...'
    logfiles = ['log/' + s for s in os.listdir('log')
                if s.endswith('.log')
                if s != 'master.log']
    #logfiles.sort()
    try:
        os.remove('log/master.log')
    except:
        pass
    master = open('log/master.log', 'a')
    for fname in logfiles:
        f = open(fname, 'r')
        master.write(f.read())
        f.close()

def list_master_log(quiet = True):
    logfile = 'log/master.log'
    print 'Listing %s ...' % logfile
    st = os.stat(logfile)
#    print '[stat called]'
    f = open(logfile, 'r')
#    print '[opened log/master.log for reading]'
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
    if not quiet:
        lst.sort()
        for n, p, score in lst:
            print 'n = %s, %s\n%s' % (n, score, p)
    total_score = sum(score for n, perm, score in lst)
    print '   Total score = %s' % total_score
    print '   "%s" last modified: %s' % (logfile,
                                         time.asctime(time.localtime(st[-2])))
    print '   %s lines' % count

if __name__ == '__main__':
    remake_master_log()
    list_master_log()

#    list_logfile()
    #db_test()
    #append_best_db_to_best_log()
