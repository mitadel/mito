# Every task reads and partitions the full mesh

For parallel (MPI) runs, every task reads the full mesh and runs the METIS partitioner on it independently, keeping only its own partition. We chose this over having one task read and partition the mesh and scatter the partitions, because it needs no mesh serialization or communication. It also gives every task a full view of the mesh, so global node numbering, node ownership and the Dirichlet boundary can be worked out locally and the same way on every task. The cost is that every task holds the whole mesh in memory, which is acceptable for the current benchmark meshes but will not scale to large meshes.

## Consequences

- METIS must be called with identical inputs and options on every task so that all tasks compute the same partitioning.
- Moving to a read-on-one-task-and-scatter scheme later would require a way to serialize meshes, and would require global node numbers to be sent along with the partitions instead of computed locally.
