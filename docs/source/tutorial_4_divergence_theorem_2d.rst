==================================
Tutorial 4 — Divergence Theorem 2D
==================================

This tutorial demonstrates numerical integration on meshes and verification of the divergence theorem in 2D.

Source Code
===========

.. literalinclude:: ../../tutorial/4_divergence_theorem_2D/main.cc
   :language: cpp
   :linenos:

Output
======

.. include:: ../_tutorial_outputs/4_divergence_theorem_2D/output.txt
   :literal:

What This Example Shows
=======================

1. **Mesh Construction**: Building a 2D triangular mesh by:

   - Creating nodes at specified coordinates
   - Inserting triangles that reference these nodes

2. **Manifolds and Integration**: 

   - Creating a **body manifold** (interior) for volume integration
   - Creating a **boundary manifold** (surface) for surface integration
   - Using Gauss quadrature for numerical integration

3. **Field Definition**: Defining a vector field on the domain:

   .. math::

      \mathbf{f}(\mathbf{x}) = x_0 x_1 \, \mathbf{e}_0 + x_0^2 \, \mathbf{e}_1

4. **Verification**: Computing:

   - Divergence on the body via volume integral
   - Field flux through the boundary via surface integral
   - Confirming they are (approximately) equal
