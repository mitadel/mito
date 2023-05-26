#! /usr/bin/env python3

import mito


vectorTuple = (2., 3., 1.)
vectorMito = mito.Vector3D(vectorTuple)

for itemMito, itemTuple in zip(vectorMito, vectorTuple):
    assert itemMito == itemTuple

print("all done!")
