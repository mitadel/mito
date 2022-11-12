#! /usr/bin/env python3

import mito
from numpy import cos


def myfunction(x):
    return eval('cos(x[0] * x[1])')


field = mito.ScalarField2D(myfunction)

# TOFIX: this line ...
manifold = mito.ManifoldTriangle2D("square.summit")
# ...should be instead:
# mesh = mito.SimplicialMesh2D("square.summit")
# manifold = mito.ManifoldTriangle2D(mesh)
integrator = mito.GaussIntegrator2Triangle2D(manifold)

N = 1
for i in range(0, N):
    result = integrator.integrate(field)

assert abs(result - 0.946083) < 1.e-3

print("all done!")
