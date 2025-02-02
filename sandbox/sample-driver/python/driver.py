# Simulation components
# - model, collection of: domain, governing equations (physics (mechanics vs thermal | constitutive behavior)), boundary conditions
# - discretization (implicit / explicit / ...)
# - solver
# - postprocessing
# - runtime context (mpi / gpus / etc...)
# - simulation controller (run / clock)

# QUESTIONS:

import mito

# look up for disk in the yaml file, fallback on mito definitions (for example we can provide a unit disk or other simple geometries)
disk = mito.Domain("disk")
# or use a custom domain for which a discretization is provided by the user
# jet_engine = mito.Domain("jet engine")

# QUESTION: how do we give the user the possibility of defining their own set of governing equations?
# define the mechanical governing equations
mechanics_equations = mito.GoverningEquations(physics="mechanics statics", constitutive="linear elastic")
# equations = mito.GoverningEquations("mechanics statics", material="steel")

thermal_equations = mito.GoverningEquations(physics="thermostatic", constitutive="fourier")
# define the thermal governing equations

# extract the boundary
circle = disk.boundary()
#
dirichlet_mechanics_bcs = mito.BoundaryConditions(domain=circle, region="labelA", type="dirichlet", value="[constant(1), constant(0)]")
#
dirichlet_thermal_bcs = mito.BoundaryConditions(domain=circle, region="labelB", type="dirichlet", value="constant(1)")

# mechanical model
mechanical_model = mito.Model(domain=disk, equations=mechanics_equations)
mechanical_model.set_boundary_conditions(dirichlet_mechanics_bcs)

# (space) discretization of mechanics model
mechanical_discretization = mito.Discretization(model=mechanical_model)
mechanical_discretization.discretize_space(type="Discontinuous Galerkin", order=2)
mechanical_discretization.discretize_time(type="Explicit Newmark", step=0.1)

# thermal model
thermal_model = mito.Model(domain=disk, equations=mechanics_equations)
thermal_model.set_boundary_conditions(dirichlet_thermal_bcs)

# (space) discretization of thermal model
thermal_discretization = mito.Discretization(model=thermal_model)
thermal_discretization.discretize_space(type="Continuous Galerkin", order=2)
thermal_discretization.discretize_time(type="Runge Kutta", order=4, step=0.1)

#
mechanical_solver = mito.Solver(type="nonlinear", backend="petsc")
thermal_solver = mito.Solver(type="linear", backend="petsc")

#
multiphysics_solver = mito.MultiphysicsSolver([mechanical_solver, thermal_solver], type="staggered", convergence_criterion=..., max_iter=..., tolerance=...)

#
mesh_writer = mito.MeshWriter(domain="disk")
displacement = mechanical_discretization.get_field('displacement')
mesh_writer.record(displacement)
temperature = thermal_discretization.get_field('temperature')
mesh_writer.record(temperature)

#
context = mito.RuntimeContext()

#
simulation = mito.SimulationController(name="mito simulation", context=context)
simulation.add_solver(multiphysics_solver)
simulation.add_writer(mesh_writer)
simulation.set_logger(type=..., frequency=...)

#
status = simulation.run()

