#! /usr/bin/python
# coding:utf-8
#
#	by seCOnDatkE, 2014.09.25 PM
#	Acknowledgements: 

from decimal import Decimal, getcontext
getcontext().prec = 100

EXP = "256.25646375475624626347548568467724626475856856857835734745846856 + 128 + 412 * (5-3) - 4 - 3 ^ 2"

def is_num(s):
	for c in s:
		if not c.isdigit() and c != '.':
			return False
	return True

def get_elements(exp):
	ret, STATUS, elements_cur = [], 'NONE', ''

	for c in exp:
		if c.isspace():
			continue
		if STATUS == 'NUMBER':
			if is_num(c):
				elements_cur += c
			else:
				ret.append(elements_cur)
				elements_cur = c
				STATUS = 'OPERATOR'
		elif STATUS != 'NUMBER':
			if STATUS != 'NONE':
					ret.append(elements_cur)
			if is_num(c):
				STATUS = 'NUMBER'
				elements_cur = c
			else:
				STATUS = 'OPERATOR'
				elements_cur = c
	if elements_cur:
		ret.append(elements_cur)

	return ret

PRIORITY = {
	'+' : 1,
	'-' : 1, 
	'*' : 2,
	'/' : 2,
	'%' : 2,
	'^' : 2,
	'(' : 0,
	')' : 0
}

CALC = {
	'+' : lambda a, b: a+b,
	'-' : lambda a, b: a-b,
	'*' : lambda a, b: a*b,
	'/' : lambda a, b: a/b,
	'%' : lambda a, b: a%b,
	'^' : lambda a, b: a**b,
}

def evalY(exp):
	elements = ['('] + get_elements(exp) + [')']
	numbers, operators = [], []

	for e in elements:
		print numbers, operators
		if is_num(e):
			numbers.append(e)
		else:
			prev_opr = len(operators) and operators[-1] or ''
			if prev_opr and e not in '()' and (PRIORITY[prev_opr] > PRIORITY[e] or prev_opr == e):
				opr, t1, t2 = operators.pop(), numbers.pop(), numbers.pop()
				numbers.append(CALC[opr](Decimal(t2), Decimal(t1)))
				operators.append(e)
			elif e == ')':
				while 1:
					opr = operators.pop()
					if opr == '(':
						break
					t1, t2 = numbers.pop(), numbers.pop()
					numbers.append(CALC[opr](Decimal(t2), Decimal(t1)))
			else:
				operators.append(e)

	return numbers.pop()

def main():
	print(EXP)
	print(evalY(EXP))

if __name__ == '__main__':
	main()
