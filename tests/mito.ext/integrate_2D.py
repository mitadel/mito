#! /usr/bin/env python3
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#

import mito
from numpy import cos


def myfunction(x):
    return eval('cos(x[0] * x[1])')


field = mito.ScalarField2D(myfunction)

# Create coordinate system
coord_system = mito.CoordinateSystem2D()
# Read mesh
mesh = mito.SimplicialMesh2D("square.summit", coord_system)
# Create manifold from mesh
manifold = mito.ManifoldTriangle2D(mesh, coord_system)
# Create integrator
integrator = mito.GaussIntegrator2Triangle2D(manifold)

N = 1
for i in range(0, N):
    result = integrator.integrate(field)

assert abs(result - 0.946083) < 1.e-3

print("all done!")
