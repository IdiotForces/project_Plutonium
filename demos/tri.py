from math import pow
from pprint import pprint

def triangle(a, b, c):
	l = [a, b, c]
	ret = { 'isosceles': False,
			'right-angle': False,
			'equilateral': False }

	for i in range(0, 3):
		if l[0]+l[1] <= l[2]:
			return False
		l.append(l.pop(0))
		l.append(l.pop(0))

		if l[0] == l[1]:
			ret['isosceles'] = True

		if pow(l[0], 2)+pow(l[1], 2) == pow(l[2], 2):
			ret['right-angle'] = True

	if len(set(l)) == 1:
		ret['equilateral'] = True

	return True, ret

def main():
	in_ = raw_input()
	a, b, c = map(int, in_.split(' '))
	pprint(triangle(a, b, c))

if __name__ == '__main__':
	status = main()