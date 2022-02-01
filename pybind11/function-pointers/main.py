#! /usr/bin/env python3

import example


def square(x):
    return eval('x * x')


integral = example.Integral(square)
print(integral.evaluate(10.0))
