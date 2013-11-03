lasers
======

Codes for modeling lasers
This code models a laser pulse in a fiber cavity. Our goal with this is to solve 
1D partial differential equations of the form du/dt = h(u). Since the computational domain
is periodic in x, we use a spectral method to solve this euqation. This reduces the problem
to the ODE dF(u)/dt = F(h(u)), where F is the fourier transform. For a series expansion,
we solve the associated ODE for each of the Fourier coefficients.

The program is organized into 3 main sections: The solver, the integrator, and the right hand side.

Solver: The solver class is not currently implemented, but it will control to simulation 
of the problem as a whole. It will deal with running the integrator and retrieving the 
solution values from the final result. For example, the solver will integrate the CNLS around
a round trip, and then apply the waveplates and polarizers.

Integrator: The integrator class integrates the right hand side of the problem in Fourier space.
It takes in the initial condition and returns the solution at a later time. The runtime
parameters, such as the number of steps to take or the maximum error, are specified in the constructor
Currently, only a fixed step 4th order runge kutta integrator is implemented.

RHS: The rhs class defines the problem being solved. It takes in the solution values and returns
the derivative at the specific point. The actual parameters that may define a specirfic problem are passed in the constructor. Currently, du/dt=const and du/dt=u are both implemented. A definition for the CNLS equations is being worked on; however, it currently has an error and the solutions increase rapidly to infinity.

All of the above classes are purely virtual, and see each other as a sort of black box.
The solver sees the integrator as an object that takes in an initial condition and returns a solution,
nothing more. Meanwhile, the integrator sees the rhs as something that just takes a solution value and returns a derivative. This makes for easy addition of new functionality. If I wanted to add the equation du/dt=u^-1, I could code a child class for for that and all the code I would replace in the main program would be that the rhs* would point to an instance of rhs_inverse instead of whatever it did before.
