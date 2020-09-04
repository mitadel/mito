#include <cmath>
#include <summit/mesh/mesh.h>
#include <summit/materials/material_library.h>
#include <summit/materials/elastic/elastic.h>
#include <summit/fem/function_space.h>
#include <summit/systems/statics_system.h>
#include <summit/solvers/linear_solver.h>

summit::vector<3> sourceTerm(const summit::vector<3>& x, summit::real t)
{
    return {x[0]*t, x[1], x[2]}; 
}

summit::vector<3> dirichletBC(const summit::vector<3>& x, summit::real t)
{
    return {0.0, 0.0, 0.0}; 
}

summit::vector<3> neumannBC(const summit::vector<3>& x, summit::real t)
{
    return {-1.0, 0.0, 0.0}; 
}

bool filterLeft(const summit::vector<3>& x)
{
    if (std::fabs(x[0]) < 1e-8)
        return true;

    return false; 
}

int main (int argc, char ** argv) 
{    
    // ------------------------------------
    // input file
    // ------------------------------------
    summit::InputFile input("input.dat");
    // print all parameters in input file
    input.Display();
    // material density 
    real rho = input.GetReal("density");
    // material Young's modulus
    real E = input.GetReal("Young's modulus");
    // material Poisson's ratio
    real nu = input.GetReal("Poisson's ratio");

    // ------------------------------------
    // mesh file
    // Remark 1: not needed after the discretization, must be somehow destroyed
    // Remark 2: the mesh file also contains the boundary 'physical' entities
    // ------------------------------------
    // instantiate a mesh object
    summit::Mesh mesh("mesh.summit"); 

    // ------------------------------------
    // material
    // Remark: would be nice to somehow define constitutive law from upper level
    // ------------------------------------
    // instantiate an elastic material
    summit::Elastic elasticMaterial(rho, E, nu, 1 /*numIntVars*/, 1 /*plain strain*/);

    /*
    stress, tangent, update     are function pointers
    parameters                  is the set of material parameters
    internalVariables           is the set of internal variables
    summit::CustomMaterial myMaterial(&stress, &tangent, &update, parameters, internalVariables);
    */

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
    // Remark: Ideally the function space should own the mesh and destroy it when done with
    //         the discretization. However, in multi-physics problems, we might need to use the same 
    //         mesh to build different sets of shape functions...
    // ------------------------------------
    // instantiate a function space object
    summit::FunctionSpace functionSpace(mesh, "CG", "P1");

    // ------------------------------------
    // system
    // ------------------------------------
    // instantiate a system binding the functionSpace (math) and the materialLibrary (physics)
    summit::System system(functionSpace, materialLibrary);
    system.AddSourceTerm(sourceTerm);
    system.AddDirichletBC("boundary A", dirichletBC);
    system.AddDirichletBC(filterLeft, dirichletBC);
    system.AddNeumannBC("boundary B", neumannBC);

    // ------------------------------------
    // VTU writer
    // ------------------------------------
    // add fields to print
    system.AddToVTU("displacement");
    system.AddToVTU("boundary");
    system.AddToVTU("forces");
    system.AddToVTU("stress");
    system.AddToVTU("analytic displacement", functorAnalytic);

    // ------------------------------------
    // solver
    // ------------------------------------
    // instantiate a linear solver
    summit::LinearSolver solver(&system);
    // solve the problem (this assembles and solves the algebraic system of equations)
    solver.Solve();
    // write the solution to output file
    system.PrintVTU("output", 0 /*step*/);

    // all done
    return 0;
}
