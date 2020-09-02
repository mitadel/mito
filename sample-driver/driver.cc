#include <cmath>
#include <summit/mesh/mesh.h>
#include <summit/materials/material_library.h>
#include <summit/materials/elastic/elastic.h>
#include <summit/fem/function_space.h>
#include <summit/systems/statics_system.h>
#include <summit/solvers/linear_solver.h>
#include <summit/systems/system_writer.h>
#include <summit/mathlib/point.h>

// radius of the hole
static real R = 0.01;
// length of the plate
static real L = 0.03;
// shear modulus
static real MU = 70e9;
// Poisson's ratio
static real NU = 0.3;
// far field sigma_xx
static real SIGMA_INF = 0.01*MU;
// tolerance for nodes localization
static real TOL_X = 1.e-8;

std::array<real> functorB(const std::array<real>& x, real t)
{
    return std::array<real>(3, 0.0);
}

typedef summit::functor std::functional(std::array<real>&(const std::array<real>& x, real t));

summit::functor sourceTerm({ return std::array<real>(3, 0.0);});

int main (int argc, char ** argv) 
{    
    // instantiate a mesh object
    summit::Mesh mesh("MESH/plate_with_hole.summit"); 
    // ---> The mesh is not needed after the discretization 
    //      Who destroys the mesh?

    // TODO: define constitutive law from upper level 
    // (stress and tangent as functions of u and gradu)
    // ------------------------------------
    // material
    // ------------------------------------
    // density (not used: this is a static calculation)
    real rho = 2500.;
    // Young's modulus
    real E = 2. * MU * (1. + NU);
    // 2D mode (0 = plane stress 2D, 1 = plain strain 2D, 2 = general 3D)
    int mode = 1;
    // instantiate an elastic material
    summit::Elastic elasticMaterial(rho, E, NU, 1 /*numIntVars*/, mode);

    // ------------------------------------
    // material library
    // ------------------------------------
    // instantiate a material library
    summit::MaterialLibrary materialLibrary;
    // add elastic material to the material library
    materialLibrary.AddMaterial(1, &elasticMaterial);
    // print all material properties in material library
    materialLibrary.Display();

    // ------------------------------------
    // function space
    // ------------------------------------
    // instantiate a function space object
    summit::FunctionSpace functionSpace(mesh, "CG", "P1");

    // ------------------------------------
    // system
    // ------------------------------------
    // instantiate a system binding the functionSpace (math) and the materialLibrary (physics)
    summit::System system(functionSpace, materialLibrary);
    system.AddSourceTerm(sourceTerm);
    system.AddDirichletBC("boundaryA", functorA);
    system.AddNeumannBC("boundaryB", functorB);

    system.AddForPrint("displacement");
    system.AddForPrint("boundary");
    system.AddForPrint("forces");
    system.AddForPrint("stress");
    system.AddForPrint("analytic displacement", functorAnalytic);
    system.WriteVTU("output", 0 /*step*/)

    // ------------------------------------
    // solver
    // ------------------------------------
    // instantiate a linear solver
    summit::LinearSolver solver(&system);
        
    // ------------------------------------
    // solution
    // ------------------------------------

    // solve the problem (this assembles and solves the linear system of equations)
    solver.Solve();

    // update the fields (e.g. stress, strain) in the system 
    system.Update(comMan);

    // write the solution to output file
    system.WriteVTU("output", 1);

    // all done
    return 0;

}
