// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxArrayToString mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '    char *str = mxArrayToString(prhs[0]);';
        '    mexPrintf(""%s"", str);';
        '}'],'mexArrayToString.c');
ilib_mex_build('libmextest', ['arrayToString', 'mexArrayToString', 'cmex'], 'mexArrayToString.c', [], 'Makelib', '', '', '');
exec('loader.sce');

arrayToString("hello world");