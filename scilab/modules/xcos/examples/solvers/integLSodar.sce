// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Run with exec("SCI/modules/xcos/examples/solvers/integLSodar.sce");

// Import the diagram and augment the ending time
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/examples/solvers/ODE_Example.xcos");
scs_m.props.tf = 30000;

solverName = ["LSodar", "BDF/Newton", "BDF/Functional", "Adams/Newton", "Adams/Functional"];

for solver = 0:4

 // Select the solver
 scs_m.props.tol(6) = solver;

 // Start the timer, launch the simulation and display time
 tic();
 try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end;
 t = toc();
 disp(t, "Time for " + solverName(solver+1) + ":");

end
