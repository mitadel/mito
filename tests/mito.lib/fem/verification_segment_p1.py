import sympy
from sympy.vector import CoordSys3D, gradient

# define the coordinate system
C = CoordSys3D('C')

# define the coordinate
x = C.x

# define the shape functions
phi0 = 1 - x
phi1 = x

# report
print("Computing the mass matrix for the 2 shape functions of a segment element:")

# compute the mass matrix via double integration
phis = [phi0, phi1]
print("Mass matrix:")
for i, phi_i in enumerate(phis):
    for j, phi_j in enumerate(phis):
        integral = sympy.integrate(phi_i * phi_j, (x, 0, 1))
        print(f"M[{i},{j}] = {integral}")

# report
print("Computing the stiffness matrix for the 2 shape functions of a segment element:")

# compute the shape functions gradients
dphi0 = gradient(phi0)
dphi1 = gradient(phi1)

# compute the stiffness matrix via double integration
dphis = [dphi0, dphi1]
print("Stiffness matrix:")
for i, dphi_i in enumerate(dphis):
    for j, dphi_j in enumerate(dphis):
        integral = sympy.integrate(dphi_i & dphi_j, (x, 0, 1))
        print(f"K[{i},{j}] = {integral}")
