===================================
Tutorial 1 — Functions & Chain Rule
===================================

This tutorial demonstrates MiTo's symbolic function framework.

Source Code
===========

.. literalinclude:: ../../tutorial/1_functions/main.cc
   :language: cpp
   :linenos:

Output
======

.. include:: ../_tutorial_outputs/1_functions/output.txt
   :literal:

What This Example Shows
=======================

1. **Symbolic Functions**: MiTo provides a library of basic functions that can be composed together symbolically.

2. **Function Composition**: The expression ``sin(x^3)`` is created by composing the ``sin`` and ``pow<3>`` functions.

3. **Automatic Differentiation**: The derivative is computed symbolically using automatic differentiation, and the chain rule is applied automatically:

   .. math::

      \frac{d(\sin(x^3))}{dx} = \cos(x^3) \cdot 3x^2

4. **Evaluation**: Both the numerical derivative and the analytical formula are evaluated at ``x = π/6`` and should match.
