#! /usr/bin/env python3
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#

import mito
from numpy import cos


def myfunction(x):
    return cos(x[0] * x[1])


field = mito.ScalarField2D(myfunction)

result = field(mito.Coordinates2D((2., 1.)))
assert abs(result - (-0.416146)) < 1.e-3

print("all done!")
