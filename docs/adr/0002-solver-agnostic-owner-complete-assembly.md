# Owners assemble their rows in full over ghost cells

In parallel runs, each task assembles over its partition plus its ghost cells (every cell that touches one of its owned nodes), and writes only the rows of its owned nodes to the linear system. MiTo decides the owned equation range for each task and gives it to the backend, instead of letting the backend split the rows. We chose this over the usual PETSc approach (each task assembles only its own cells and passes contributions to rows it doesn't own through `ADD_VALUES`, which PETSc sends at assembly time) so that the parallel implementation doesn't depend on any one solver: a backend only has to accept an explicit owned row range and never has to exchange matrix or vector entries between tasks. The cost is that ghost cells are assembled twice (by their own task and by each neighbouring owner). Because every task holds the full mesh (ADR-0001), this needs no communication during assembly.

## Consequences

- Don't "simplify" assembly to rely on a backend's handling of rows owned by other tasks: that brings the dependence on the solver back.
- Integrals of quantities computed after the solve (norms, for example) must cover the task's partition only, never its ghost cells, or ghost cells would be counted twice.
- Values at nodes a task doesn't own are sent from their owner through `CommunicationManager`, not through the backend.
