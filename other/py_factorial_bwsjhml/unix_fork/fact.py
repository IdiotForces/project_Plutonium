import os
import sys

COUNT_PROC = 50
FACN = 100

rp, wp = os.pipe()
r = os.fdopen(rp, 'r')
w = os.fdopen(wp, 'w')

def fact_itr(start, end):
    ret = start < end and start or 1
    for i in xrange(start+1, end):
        ret *= i
    return ret

def fac_split(src, count):
    split_size = src / count
    splits = [ (i*split_size+1, (i+1)*split_size+1) for i in xrange(0, count-1) ]
    splits.append(((count-1)*split_size+1, src))
    return splits

def subproc_child(args):
        w.write(str(fact_itr(args[0], args[1]))+'\n')
        w.flush()
        sys.exit(0)

def main():
    print(reduce(lambda x, y: x*y, [fact_itr(t[0], t[1]) for t in fac_split(4, COUNT_PROC)])*4)
    print("PID %d: starting..." % os.getpid())
    splits = fac_split(FACN, COUNT_PROC)
    for split in splits:

        pid = os.fork()
        if pid:
            print("Parent: Forked process %d." % pid)
        else:
            subproc_child(split)
            return

    ret = 1
    for i in xrange(0, COUNT_PROC):
        ret *= long(r.readline())
    ret *= FACN

    print("PID %d: exiting..." % os.getpid())
    return

if __name__  == '__main__':
    main()

def start(x):
    global FACN
    FACN = x
    main()