#! /usr/bin/env python3

import example


def square(x, t):
    return eval('x[0] * x[1] + t')


integral = example.Integral(square)
x = example.Vector3D([10., 20., 30.])
print(integral.evaluate(x, 1.0))
