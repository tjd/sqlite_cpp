# app1.py

appfile = 'app.txt'

if __name__ == '__main__':
    f = open(appfile, 'a')
    for i in range(10000000):
        f.write('app1 sets a %s\n' % i)
    f.write('app1 done\n')
