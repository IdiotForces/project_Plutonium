
import sys
import itertools

if __name__ == '__main__':

    time_iteration = (len(sys.argv) >= 2) and int(sys.argv[1]) or 10000
    
    n_array = xrange(0, time_iteration)
    ret = sum(itertools.imap(lambda n: pow(-1, n) * (1.0 / (2*n+1)), n_array))
    print('pi: %.8f' % (ret*4))

