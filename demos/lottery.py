import random

LEN = 12

a = range(1, LEN)

# maybe we need an O(n) algorithm
# 	which caster written by Caster@fundamental
# 	please refer to project_K/lottery
def lottery():
    return a.pop(random.randint(0, len(a)-1))
