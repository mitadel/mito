import numpy as np
import matplotlib.pyplot as plt

factor_theta = 1
factor_phi = 10

xi = np.linspace(0.01, np.pi / 2, 500) / factor_theta

# parametrization: (r, theta, phi) = (1, xi, 2*xi)
r = 1.0
theta = factor_theta * xi
phi = factor_phi * xi

# convert to Cartesian for plotting
x = r * np.sin(theta) * np.cos(phi)
y = r * np.sin(theta) * np.sin(phi)
z = r * np.cos(theta)

fig = plt.figure(figsize=(7, 7))
ax = fig.add_subplot(111, projection="3d")

# translucent sphere
u = np.linspace(0, 2 * np.pi, 60)
v = np.linspace(0, np.pi, 40)
xs = np.outer(np.sin(v), np.cos(u))
ys = np.outer(np.sin(v), np.sin(u))
zs = np.outer(np.cos(v), np.ones_like(u))
ax.plot_surface(xs, ys, zs, alpha=0.12, color="steelblue")

ax.plot(x, y, z, color="crimson", linewidth=2.5, label=r"$(r,\theta,\varphi)=(1,\,\xi,\,2\xi)$")
ax.scatter(*[[c[0]] for c in (x, y, z)], color="green", s=40, zorder=5, label=r"$\xi=0$")
ax.scatter(*[[c[-1]] for c in (x, y, z)], color="black", s=40, zorder=5, label=r"$\xi=\pi/2$")

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("z")
ax.set_title("Spiral on the unit sphere")
ax.legend(loc="upper left")
ax.set_box_aspect([1, 1, 1])

plt.tight_layout()
plt.savefig("spherical_spiral.png", dpi=150)
plt.show()
