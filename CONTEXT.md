# MiTo

MiTo is a library for solving PDEs with the finite element method on simplicial meshes. This glossary covers the language used for running those solves in parallel.

## Parallel execution

**Task**:
One of the parallel processes taking part in a run, identified by its task id, an integer from 0 to the number of tasks minus 1.
_Avoid_: Rank, process, worker

**Partition**:
The subset of a mesh's cells assigned to a single task by the mesh partitioner. Partitions do not overlap: every cell belongs to exactly one partition, and each task holds exactly one partition.
_Avoid_: Subdomain, local mesh, chunk

**Ghost cell**:
A cell outside a task's partition that touches one of the task's owned nodes. The task assembles over ghost cells so that each of its owned rows is complete.
_Avoid_: Halo cell, overlap cell

**Shared node**:
A discretization node touched by cells from more than one partition.
_Avoid_: Interface node, halo node, boundary node (that term is reserved for the physical boundary)

**Owner**:
The single task responsible for a node's equation: the lowest-numbered task whose partition touches the node.
_Avoid_: Master, home rank

**Owned node**:
A node whose owner is the current task. Every node is owned by exactly one task.

**Global ID**:
An identifier for a mesh vertex or discretization node that is the same on every task and does not depend on memory addresses.
_Avoid_: Global index, tag
