import fact
import factn

def ttime(foo):
	import time
	t = time.time()
	foo()
	print(time.time()-t)
	return time.time()-t

if __name__ == '__main__':
	FACN = long(input())
	ttime(lambda :fact.start(FACN))
	t = raw_input('Press <return> to continue...')
	ttime(lambda :factn.start(FACN))