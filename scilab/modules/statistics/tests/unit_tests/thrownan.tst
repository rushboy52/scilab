// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

x=[0.2113249 %nan 0.6653811 %nan;0.7560439 0.3303271 0.6283918 %nan];
[nonan,numb]=thrownan(x);
expected = [0.2113249;0.7560439;0.3303271;0.6653811;0.6283918];
assert_equal ( nonan , expected );
assert_equal ( numb , [1,2,4,5,6] );
//
[nonan,numb]=thrownan([]);
assert_equal ( nonan , [] );
assert_equal ( numb , [] );
