// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8775 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8775
//
// <-- Short Description -->
// The karmarkar function might diverge toward a non-optimal point.
//

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

// An unbounded problem.
Aeq = [
 2 -2 -1 1 0
-1 -4  1 0 1
];
beq = [-1;-1];
c = [2;9;3;0;0];
x0 = [0.2;0.7;1;1;1];
[xopt,fopt,exitflag]=karmarkar(Aeq,beq,c,x0,0,0.999);
MY_assert_equal ( exitflag , -2 );
