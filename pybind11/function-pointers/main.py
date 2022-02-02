#! /usr/bin/env python3

import example


def square(x):
    return eval('x[0] * x[1]')


integral = example.Integral(square)
x = example.Vector3D([10., 20., 30.])
print(integral.evaluate(x))
