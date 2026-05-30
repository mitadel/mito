========================================
Tutorial 5 — Divergence Theorem 2D in 3D
========================================

This tutorial extends Tutorial 4 into 3D space.

Source Code
===========

.. literalinclude:: ../../tutorial/5_divergence_theorem_2D_in_3D/main.cc
   :language: cpp
   :linenos:

Output
======

.. include:: ../_tutorial_outputs/5_divergence_theorem_2D_in_3D/output.txt
   :literal:

What This Example Shows
=======================

1. **3D Coordinates**: Extending the 2D example to 3D:

   - Coordinate system: 3D Cartesian coordinates ``(x_0, x_1, x_2)``
   - Nodes created with three coordinates

2. **Embedded Surfaces**: The mesh is a 2D triangular surface **embedded** in 3D:

   - All nodes have ``x_2 = 0`` (lie on the z=0 plane)
   - The surface normal is ``e_2`` (pointing in the z-direction)

3. **Submanifolds**: Using the ``submanifold`` API to:

   - Specify the body manifold with a normal vector
   - Specify the boundary manifold with both the surface normal and edge tangent normals

4. **Field Definition**: The same vector field as Tutorial 4, but now:

   - Defined in 3D space (but only uses ``x_0`` and ``x_1``):
   
      :math:`\mathbf{f}(\mathbf{x}) = x_0 x_1 \, \mathbf{e}_0 + x_0^2 \, \mathbf{e}_1`
   
   - Integrated over a surface in 3D

5. **Divergence Theorem on a Surface**: Verifying the 2D divergence theorem for the surface in 3D:

   .. math::

      \int_S \nabla_S \cdot \mathbf{f} \, dS = \oint_{\partial S} \mathbf{f} \cdot \mathbf{n} \, d\ell

   where :math:`\nabla_S` is the intrinsic divergence on the surface.
