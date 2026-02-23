====================
Tutorial 2 — Tensors
====================

This tutorial demonstrates tensor operations in MiTo.

Source Code
===========

.. literalinclude:: ../../tutorial/2_tensors/main.cc
   :language: cpp
   :linenos:

Output
======

.. include:: ../_tutorial_outputs/2_tensors/output.txt
   :literal:

What This Example Shows
=======================

1. **Tensor Creation**: MiTo provides two ways to create tensors:

   - **Explicit initialization**: ``vector_t<2>{ 1.0, 1.0 }``
   - **Basis element composition**: ``e_0<2> + e_1<2>``

2. **Matrices and Linear Operations**: Matrices are constructed from basis elements (``e_00``, ``e_01``, etc.) or via explicit initialization.

3. **Quadratic Forms**: The expression ``a^T * B * a`` computes the quadratic form, demonstrating tensor contraction and multiplication.
