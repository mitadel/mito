# ADR-0001 - Design of time integrators
## Context
For semi-discrete transient problems, we require integrators that march through time. These can range from simple integration schemes such as (non-adaptive) explicit Euler to adaptive, high-order Runge-Kutta schemes. 

Compared to a steady-state problem, the following complexities are introduced:
- The solution changes at each timestep. 
- The constraints may be functions of time. 
- Some schemes may require storage of intermediate states.
- Initialization of the initial solution is required. 
- In contrast to steady-state problems, the required matrix solve varies wildly between different 
- In case of explicit schemes, a diagonal (lumped) mass matrix is required. 

Additionally, in the near future, we might want to couple our simulation unit (SU) in a co-simulation scenario with another SU, bringing additional complexities that might need to be addressed at some point:
- In this case, we need to expose the output of our simulation. 
- Moreover, we need to accommodate the


## Decision
Transient solvers 

For now we focus on the case where we deal with a 




## Consequences
For now, we have assumed constant time-steps for the integration schemes. How adaptive schemes should be implemented, is an open question. Specifically, the following would need to be addressed:
- Does the interface need to be expanded? That is, do we require more methods? Do existing methods need additional arguments?
- Do we want to house the capability for an adaptive timestep inside the existing `ExplicitEuler`-implementation? Or do we define a separate `AdaptiveExplicitEuler`-class that handles this. 

Moreover, in the near future, we might want to couple our simulation unit (SU) in a co-simulation scenario with another SU, bringing additional complexities that might need to be addressed at some point:
- The constraints follow from external inputs.
- These inputs may not always be provided at the timestamps required by the time-integrations scheme, requiring some form of intra-/extrapolation.
- The output needs to exposed to the external SU too.

Thus, the questions that need to be addressed then, are:
- How do we deal with these complexities?
- Do we turn these into a different type of solvers? On one hand, the capability to import outputs from different SUs may seem out of place for monolithic solvers. On the other, at least at first glance, a non-co-simulation scenario is essentially a co-simulation with only a single SU (that is, ) 




## References
[1] Cláudio Gomes, Casper Thule, David Broman, Peter Gorm Larsen, and Hans Vangheluwe. 2018. Co-Simulation: A Survey. ACM Comput. Surv. 51, 3, Article 49 (May 2019), 33 pages. https://doi.org/10.1145/3179993
[2] Modelica Association Project. (2024). Functional Mock-up Interface (FMI): Version 3.0.2. Modelica Association. Available online: https://fmi-standard.org/docs/3.0.2/ 