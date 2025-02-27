#include <petscksp.h>

int
main(int argc, char ** args)
{
    Vec x, b;    // solution, right-hand-side vectors
    Mat A;       // matrix
    KSP ksp;     // linear solver context
    PetscErrorCode ierr;
    PetscInt n = 10;    // size of the linear system
    PetscMPIInt size;

    PetscInitialize(&argc, &args, (char *) 0, "Solving a linear system with KSP");

    MPI_Comm_size(PETSC_COMM_WORLD, &size);
    if (size != 1) {
        SETERRQ(PETSC_COMM_WORLD, 1, "This is a uniprocessor example only!");
    }

    // Create vectors
    ierr = VecCreate(PETSC_COMM_WORLD, &x);
    CHKERRQ(ierr);
    ierr = VecSetSizes(x, PETSC_DECIDE, n);
    CHKERRQ(ierr);
    ierr = VecSetFromOptions(x);
    CHKERRQ(ierr);
    ierr = VecDuplicate(x, &b);
    CHKERRQ(ierr);

    // Create matrix
    ierr = MatCreate(PETSC_COMM_WORLD, &A);
    CHKERRQ(ierr);
    ierr = MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, n);
    CHKERRQ(ierr);
    ierr = MatSetFromOptions(A);
    CHKERRQ(ierr);
    ierr = MatSetUp(A);
    CHKERRQ(ierr);

    // Set matrix entries
    for (PetscInt i = 0; i < n; i++) {
        PetscScalar v = 2.0;
        ierr = MatSetValue(A, i, i, v, INSERT_VALUES);
        CHKERRQ(ierr);
        if (i > 0) {
            v = -1.0;
            ierr = MatSetValue(A, i, i - 1, v, INSERT_VALUES);
            CHKERRQ(ierr);
        }
        if (i < n - 1) {
            v = -1.0;
            ierr = MatSetValue(A, i, i + 1, v, INSERT_VALUES);
            CHKERRQ(ierr);
        }
    }

    // Assemble matrix
    ierr = MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    CHKERRQ(ierr);
    ierr = MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
    CHKERRQ(ierr);

    // View the matrix
    ierr = MatView(A, PETSC_VIEWER_STDOUT_WORLD);
    CHKERRQ(ierr);

    // Set right-hand-side vector
    ierr = VecSet(b, 1.0);
    CHKERRQ(ierr);

    // View the right-hand-side
    ierr = VecView(b, PETSC_VIEWER_STDOUT_WORLD);
    CHKERRQ(ierr);

    // Create linear solver context
    ierr = KSPCreate(PETSC_COMM_WORLD, &ksp);
    CHKERRQ(ierr);
    ierr = KSPSetOperators(ksp, A, A);
    CHKERRQ(ierr);
    ierr = KSPSetFromOptions(ksp);
    CHKERRQ(ierr);

    // Solve the linear system
    ierr = KSPSolve(ksp, b, x);
    CHKERRQ(ierr);

    // View the solution
    ierr = VecView(x, PETSC_VIEWER_STDOUT_WORLD);
    CHKERRQ(ierr);

    // Clean up
    ierr = KSPDestroy(&ksp);
    CHKERRQ(ierr);
    ierr = VecDestroy(&x);
    CHKERRQ(ierr);
    ierr = VecDestroy(&b);
    CHKERRQ(ierr);
    ierr = MatDestroy(&A);
    CHKERRQ(ierr);

    PetscFinalize();
    return 0;
}