#include <cmath>
#include <mito/mesh/mesh.h>
#include <mito/materials/material_library.h>
#include <mito/materials/elastic/elastic.h>
#include <mito/fem/function_space.h>
#include <mito/systems/statics_system.h>
#include <mito/solvers/linear_solver.h>

mito::vector<3> sourceTerm(const mito::vector<3>& x, mito::real t)
{
    return {x[0]*t, x[1], x[2]}; 
}

mito::vector<3> dirichletBC(const mito::vector<3>& x, mito::real t)
{
    return {0.0, 0.0, 0.0}; 
}

mito::vector<3> neumannBC(const mito::vector<3>& x, mito::real t)
{
    return {-1.0, 0.0, 0.0}; 
}

bool filterLeft(const mito::vector<3>& x)
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
    mito::InputFile input("input.dat");
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
    mito::Mesh mesh("mesh.mito"); 

    // ------------------------------------
    // material
    // Remark: would be nice to somehow define constitutive law from upper level
    // ------------------------------------
    // instantiate an elastic material
    mito::material::Elastic elasticMaterial(rho, E, nu, 1 /*numIntVars*/, 1 /*plain strain*/);
    // or...
    auto material = mito::material::elastic(rho, E, nu);

    /*
    stress, tangent, update     are function pointers
    parameters                  is the set of material parameters
    internalVariables           is the set of internal variables
    mito::CustomMaterial myMaterial(&stress, &tangent, &update, parameters, internalVariables);
    */

    // ------------------------------------
    // material library
    // ------------------------------------
    // instantiate a material library
    mito::MaterialLibrary materialLibrary();
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
    mito::FunctionSpace functionSpace(mesh, "CG", "P1");

    // ------------------------------------
    // system
    // ------------------------------------
    // instantiate a system binding the functionSpace (math) and the materialLibrary (physics)
    mito::System system(functionSpace, materialLibrary);
    system.AddSourceTerm(sourceTerm);
    system.AddDirichletBC("boundary A", dirichletBC);
    system.AddDirichletBC(filterLeft, dirichletBC);
    system.AddNeumannBC("boundary B", neumannBC);

    // define the load to apply to the system (e.g. BCs and source terms)
    mito::Load 
    system.Add(load);

    // ------------------------------------
    // VTU writer
    // ------------------------------------
    // add fields to print
    auto vtu = vtuWriter("output");
    vtu.Record("boundary") 
    vtu.Record("forces") 
    vtu.Record("stress") 
    vtu.Record("analytic displacement", functorAnalytic); 
    system.AddToWriter(vtu);

    // ------------------------------------
    // solver
    // ------------------------------------
    // instantiate a linear solver
    mito::LinearSolver solver;
    solver.Add(system);

    time loop {
        // solve the problem (this assembles and solves the algebraic system of equations)
        solver.Step(dt);
        system.Write(t);
    }

    // all done
    return 0;
}
