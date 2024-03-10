#! /usr/bin/env python3
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#

import mito


vectorTuple = (2., 3., 1.)
vectorMito = mito.Coordinates3D(vectorTuple)

for itemMito, itemTuple in zip(vectorMito, vectorTuple):
    assert itemMito == itemTuple

print("all done!")
