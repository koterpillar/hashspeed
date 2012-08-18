#!/usr/bin/env python

from pprint import pprint
from random import random
import string
import sys

items = string.lowercase

def item():
    return items[int(random() * len(items))]

def width():
    return int(random() * 10)

def generate(depth):
    if depth == 0:
        return item()
    else:
        result = {}
        for i in range(1, width()):
            result[item()] = generate(depth - 1)
        return result

def count(tree):
    accum = {it: 0 for it in items}
    def count_accum(tree):
        if type(tree) is str:
            accum[tree] += 1
        else:
            for k, v in tree.items():
                accum[k] += 1
                count_accum(v)
    count_accum(tree)
    return accum

tree = generate(int(sys.argv[1]))
counts = count(tree)
pprint(counts)
