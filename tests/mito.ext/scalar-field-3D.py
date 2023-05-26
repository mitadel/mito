#! /usr/bin/env python3

import mito
from numpy import cos


def myfunction(x):
    return eval('cos(x[0] * x[1]) + x[2]')


field = mito.ScalarField3D(myfunction)

result = field(mito.Vector3D((2., 1., 3.)))
assert abs(result - 2.583853) < 1.e-3

print("all done!")
