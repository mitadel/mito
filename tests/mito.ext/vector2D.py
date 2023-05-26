#! /usr/bin/env python3

import mito


vectorTuple = (2., 3.)
vectorMito = mito.Vector2D(vectorTuple)

for itemMito, itemTuple in zip(vectorMito, vectorTuple):
    assert itemMito == itemTuple

print("all done!")
