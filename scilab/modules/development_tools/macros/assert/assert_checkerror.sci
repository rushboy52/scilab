// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [flag,errmsg] = assert_checkerror ( varargin )
    //  Check that an instruction produces the expected error.

    [lhs,rhs]=argn()
    if ( and(rhs <> [2 3] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_checkerror" , 2 , 3 )
        error(errmsg)
    end
    //
    // Get arguments
    instr = varargin(1)
    expectedmsg = varargin(2)
    expectederrnb = argindefault ( rhs , varargin , 3 , [] )
    //
    // Check types of variables
    if ( typeof(instr) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix of strings expected.\n") , "assert_checkerror" , 1 )
        error(errmsg)
    end
    if ( typeof(expectedmsg) <> "string" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix of strings expected.\n") , "assert_checkerror" , 2 )
        error(errmsg)
    end
    if ( typeof(expectederrnb) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_checkerror" , 3 )
        error(errmsg)
    end  
    //
    // Check sizes of variables
    if ( size(instr,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 1 , 1 , 1 )
        error(errmsg)
    end
    if ( size(expectedmsg,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 2 , 1 , 1 )
        error(errmsg)
    end
    if ( expectederrnb <> [] ) then
        if ( size(expectederrnb,"*") <> 1 ) then
            errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 3 , 1 , 1 )
            error(errmsg)
        end
    end
    //
    // Check values of variables
    if ( expectederrnb <> [] ) then
        if ( expectederrnb < 0 ) then
            errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Non-negative integers expected.\n"  ) , "assert_checkerror" , 3 )
            error(errmsg)
        end
    end
    //
    // Proceed...
    compierr = execstr(instr,"errcatch")
    //
    // Check that an error is produced
    if ( compierr == 0 ) then
        localstr = gettext ( "%s: No error was produced while evaluating ""%s"".")
        errmsg = sprintf ( localstr , "assert_checkerror" , instr )
        error(errmsg)
    end
    //
    // Get the error
    compmsg = lasterror()
    //
    // Initialize output arguments
    flag = %t
    errmsg = ""
    //
    // Check the error message
    if ( expectedmsg <> compmsg ) then
        flag = %f
        localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".")
        errmsg = msprintf(localstr,"assert_checkerror",expectedmsg,compmsg)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            error ( errmsg )
        else
            return
        end
    end
    if ( expectederrnb <> [] ) then
        //
        // Check the error number
        if ( expectederrnb <> compierr ) then
            flag = %f
            localstr = gettext("%s: Assertion failed: expected error number = %d while computed error number = %d.")
            errmsg = msprintf(localstr,"assert_checkerror",expectederrnb,compierr)
            if ( lhs < 2 ) then
                // If no output variable is given, generate an error
                error ( errmsg )
            else
                return
            end
        end
    end
endfunction
function argin = argindefault ( rhs , vararglist , ivar , default )
    // Returns the value of the input argument #ivar.
    // If this argument was not provided, or was equal to the 
    // empty matrix, returns the default value.
    if ( rhs < ivar ) then
        argin = default
    else
        if ( vararglist(ivar) <> [] ) then
            argin = vararglist(ivar)
        else
            argin = default
        end
    end
endfunction
