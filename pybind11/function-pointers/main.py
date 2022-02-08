#! /usr/bin/env python3

import example
from numpy import cos

def myfunction(x):
    return eval('cos(x[0] * x[1])')

# TOFIX: hardcoded C++ cos(x[0] * x[1]) function
field = example.ScalarField2D()
#field = example.ScalarField2D(myfunction)
#x = example.Vector2D([10., 20.])
#print(field(x))

elementset = example.ElementSetTriangle2D("square.summit")
integrator = example.GaussIntegrator2Triangle2D(elementset)

N = 10000
for i in range(0, N):
    result = integrator.integrate(field)

#print(result)
