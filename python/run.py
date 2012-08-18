#!/usr/bin/env python

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

tree = generate(int(sys.argv[1]))
