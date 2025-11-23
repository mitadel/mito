import sympy
from sympy.vector import CoordSys3D, gradient

# define the coordinate system
C = CoordSys3D('C')

# define the scalar field
x, y, _ = C.base_scalars()

# define the shape functions
phi0 = (1 - x - y) * (1 - 2*x - 2*y)
phi1 = x * (2*x - 1)
phi2 = y * (2*y - 1)
phi3 = 4 * x * (1 - x - y)
phi4 = 4 * x * y
phi5 = 4 * y * (1 - x - y)

# report
print("Computing the mass matrix for the 6 shape functions of a triangular element:")

# compute the mass matrix via double integration
phis = [phi0, phi1, phi2, phi3, phi4, phi5]
print("Mass matrix:")
for i, phi_i in enumerate(phis):
    for j, phi_j in enumerate(phis):
        integral = sympy.integrate(phi_i * phi_j, (y, 0, 1-x), (x, 0, 1))
        print(f"M[{i},{j}] = {integral}")

# report
print("Computing the stiffness matrix for the 6 shape functions of a triangular element:")

# compute the shape functions gradients
dphi0 = gradient(phi0)
dphi1 = gradient(phi1)
dphi2 = gradient(phi2)
dphi3 = gradient(phi3)
dphi4 = gradient(phi4)
dphi5 = gradient(phi5)

# compute the stiffness matrix via double integration
dphis = [dphi0, dphi1, dphi2, dphi3, dphi4, dphi5]
print("Stiffness matrix:")
for i, dphi_i in enumerate(dphis):
    for j, dphi_j in enumerate(dphis):
        integral = sympy.integrate(dphi_i & dphi_j, (y, 0, 1-x), (x, 0, 1))
        print(f"K[{i},{j}] = {integral}")
