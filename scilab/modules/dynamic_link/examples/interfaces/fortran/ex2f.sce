// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex2f.f'];
ilib_build('gw_ex2f',['ex2f','C2F(intex2f)'],files,[]);

exec loader.sce;

// Creating the variable C 
ex2f();
if C<>[10,20,30] then pause,end

