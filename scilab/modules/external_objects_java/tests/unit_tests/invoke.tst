// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport java.lang.String;

s = String.new("Hello JIMS !!");
s1 = s.toUpperCase();
s2 = s.toLowerCase();
[S1 S2] = junwrap(s1, s2);

assert_checkequal(S1,"HELLO JIMS !!");
assert_checkequal(S2,"hello jims !!");
