# topswop.py

import random
import sys

n_vals = [  2,  3,  5,  7, 11, 
           13, 17, 19, 23, 29, 
           31, 37, 41, 43, 47, 
           53, 59, 61, 67, 71,
           73, 79, 83, 89, 97];

def make_file_name(n):
    return 'best_so_far/topswop_num_%s.txt' % n

def make_all_num_files():
    print 'Are you you want to do this?'
    print 'All existing number files will be deleted!'
    r = random.randint(100, 1000)
    ans = raw_input('Enter %s to go ahead: ' % r)
    if ans != str(r):
        print 'Wrong number: nothing done'
        return
    
    for n in n_vals:
        f = open(make_file_name(n), 'w')
        f.write('0\n')
        f.write(', '.join(str(i) for i in range(1, n+1)))

def perm_score(perm):
    count = 0
    while perm[0] != 1:
        f = perm[0]
        top = perm[:f]
        top.reverse()
        perm = top + perm[f:]
        count += 1
    return count
        
def get_num_file(n):
    assert n in n_vals
    f = open(make_file_name(n), 'r')
    score = int(f.next())
    perm = [int(i) for i in f.next().split(', ')]
    assert score == perm_score(perm), '%s\nscore = %s, perm_score = %s' % (perm, score, perm_score(perm))
    return score, perm

def gen_num_report():
    total_score = 0
    submit = []
    for n in n_vals:
        score, perm = get_num_file(n)
        total_score += score
        submit.append(', '.join(str(i) for i in perm))
    print
    print 'Submit the following for a score of %s: ' % total_score
    print ';\n\n'.join(submit)


def list_current_best_scores():
    print ' n  score'
    print '========='
    total_score = 0
    for n in n_vals:
        score, perm = get_num_file(n)
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

    #print get_num_file(17)

