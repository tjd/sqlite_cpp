# good.py

def good_file(n): return '%s.log' % n

# def make_files():
#     for i in range(14, 97+1):
#         f = open(good_file(i), 'w')

def remove_dup_lines(n):
    # read all the ones of good_file(n) into a set, thus removing
    # duplicates
    f = open(good_file(n), 'r')
    unique = set()
    for line in f:
        #print 'adding %s' % line
        unique.add(line)
        
    # write the unique lines back to good_file(n)
    f = open(good_file(n), 'w')
    for line in unique:
        #print 'writing %s' % line
        f.write(line )

def remove_all_dup_lines():
    for i in range(14, 97+1):
        remove_dup_lines(i)
        
if __name__ == '__main__':
    remove_all_dup_lines()
