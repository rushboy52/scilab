// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 INRIA Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4715 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4715
//
// <-- Short Description -->
// 
// 
load(SCI+'/modules/scicos/tests/nonreg_tests/bug_4715.cos');
%scicos_context.a = 0.1;
%scicos_context.b = 0.1;
global AA
Info = scicos_simulate(scs_m,list(),%scicos_context,'nw');
res = AA;
clearglobal AA;
if abs(res-1.56012845688924151)>1d-3 then pause,end