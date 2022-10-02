#! /usr/bin/env python3

import mito.extensions.mito as mito

def myfunction(x):
    return eval('cos(x[0] * x[1])')

field = mito.ScalarField2D(myfunction)
#x = mito.Vector2D([10., 20.])
#print(field(x))

elementset = mito.ManifoldTriangle2D("square.summit")
integrator = mito.GaussIntegrator2Triangle2D(elementset)

N = 1
for i in range(0, N):
    result = integrator.integrate(field)

assert abs(result - 0.946083) < 1.e-3
