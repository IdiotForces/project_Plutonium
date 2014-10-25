import pprint

def inssort(src):
	for i in xrange(1, len(src)):
		for j in xrange(i, 0, -1):
			if src[j-1] > src[j]:
				src[j-1], src[j] = src[j], src[j-1]
			else:
				break
		pprint.pprint(src)

def shellsort(src, threshold=4):
	if threshold == 0:
		return src
	print('shell sorting... %d' % threshold)
	for i in xrange(1, len(src)):
		for j in xrange(i, 0, -threshold):
			if src[j-threshold] > src[j]:
				src[j-threshold], src[j] = src[j], src[j-threshold]
			else:
				break
		pprint.pprint(src)
	shellsort(src, threshold/2)

def inssort_shell(src, start, end, incr):
	print('shell sorting... %d %d %d' % (start, end, incr))
	for i in xrange(incr, end, incr):
		for j in xrange(i, incr-1, -incr):
			if src[start+j-incr] > src[start+j]:
				src[start+j-incr], src[start+j] = src[start+j], src[start+j-incr]
		pprint.pprint(src)

def shellsort_2(src, incr=4):
	i = incr
	while i >= 2:
		for j in xrange(0, i):
			inssort_shell(src, j, len(src)-j, i)
		i /= 2
	inssort_shell(src, 0, len(src), 1)

def quicksort_part(src, val, left, right):
	while True:
		while left <= right and src[left] < val:
			left += 1
		while left <= right and src[right] > val:
			right -= 1
		print(left, right)
		if left >= right:
			break
		src[left], src[right] = src[right], src[left]
	return right

def quicksort(src, start=-1, end=-1):

	if start == -1:
		start = 0
	if end == -1:
		end = len(src)-1
	if start >= end:
		return src

	idx = quicksort_part(src, src[start], start+1, end)
	t = src[idx]
	src[start], src[idx] = src[idx], src[start]
	print(start, end, idx, t)
	pprint.pprint(src)
	quicksort(src, start, idx-1)
	quicksort(src, idx+1, end)

if __name__ == '__main__':
	# [2, 4, 6, 8, 10, 12, 16, 18, 20, 28, 30]
	arr = [12,2,16,30,8,28,4,10,20,6,18]
	# pprint.pprint(arr)
	shellsort_2(arr)
	# quicksort(arr)
	# pprint.pprint(arr)
