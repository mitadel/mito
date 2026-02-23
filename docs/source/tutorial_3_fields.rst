===================
Tutorial 3 — Fields
===================

This tutorial demonstrates spatial fields and differential operators.

Source Code
===========

.. literalinclude:: ../../tutorial/3_fields/main.cc
   :language: cpp
   :linenos:

Output
======

.. include:: ../_tutorial_outputs/3_fields/output.txt
   :literal:

What This Example Shows
=======================

1. **Coordinate Functions**: Accessing spatial coordinates ``x_0`` and ``x_1`` as functions suitable for field composition.

2. **Vector Fields**: Building a vector field by composing trigonometric functions with spatial coordinates:

   .. math::

      \mathbf{f}(\mathbf{x}) = \sin(x_0 x_1) \mathbf{e}_0 + \cos(x_0 x_1) \mathbf{e}_1

3. **Differential Operators**: Computing:

   - **Gradient** (``∇``): produces a rank-2 tensor field
   - **Divergence** (``∇·``): produces a scalar field
   - **Transposition**: combining operators to form compound expressions

4. **Point Evaluation**: Evaluating the resulting field expressions at specific points in space.
