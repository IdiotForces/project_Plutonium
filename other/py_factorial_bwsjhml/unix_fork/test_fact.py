import fact
import factn

def ttime(foo):
	import time
	t = time.time()
	foo()
	print(time.time()-t)
	return time.time()-t

def facts(facn):
	ret = fact.start(FACN)
	# print(ret)
	sret = str(ret)
	print(len(sret))

if __name__ == '__main__':
	FACN = long(input())
	# ttime(lambda :fact.start(FACN))
	ttime(lambda :facts(FACN))
	t = raw_input('Press <return> to continue...')
	# ttime(lambda :factn.start(FACN))