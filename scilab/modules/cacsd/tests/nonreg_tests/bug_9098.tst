// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 9098 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9098
//
// <-- Short Description -->
// demo CACSD --> Robust control failed on Windows 64 bit
// decrease optimization on sci_f_linmeq.f

s = poly(0,'s');
[N, M] = lcf(tf2ss(1/s^3));
nk = hankelsv([N, M]);
assert_checktrue(size(nk,'*') == 6);