// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for msprintf function
// =============================================================================

// format '%f'
// =============================================================================

if msprintf("%f",-35)                <> "-35.000000"        then pause,end

if msprintf("%f",35.55)              <> "35.550000"         then pause,end
if msprintf("%f",0.00433)            <> "0.004330"          then pause,end
if msprintf("%f",0.0000000345456)    <> "0.000000"          then pause,end
if msprintf("%f",1112423453)         <> "1112423453.000000" then pause,end

if msprintf("%15f",-35)              <> "     -35.000000"   then pause,end
if msprintf("%15f",0.00433)          <> "       0.004330"   then pause,end
if msprintf("%15f",0.0000000345456)  <> "       0.000000"   then pause,end
if msprintf("%15f",1112423453)       <> "1112423453.000000" then pause,end

if msprintf("%.1f" ,-35)             <> "-35.0"             then pause,end
if msprintf("%.0f" ,-35)             <> "-35"               then pause,end
if msprintf("%#.0f",-35)             <> "-35."              then pause,end
if msprintf("%.1f" ,0.00433)         <> "0.0"               then pause,end
if msprintf("%.15f",0.0000000345456) <> "0.000000034545600" then pause,end
if msprintf("%.1f" ,11124234534545)  <> "11124234534545.0"  then pause,end

// format '%g'
// =============================================================================

if msprintf("%g" ,-35)               <> "-35"             then pause,end
if msprintf("%g" ,35.55)             <> "35.55"           then pause,end
if msprintf("%g" ,35.551234567890)   <> "35.5512"         then pause,end
if msprintf("%+g",35.551234567890)   <> "+35.5512"        then pause,end
if msprintf("%g" ,0.00433)           <> "0.00433"         then pause,end

if getos() == 'Windows' then
  if msprintf("%g" ,0.0000000345456)   <> "3.45456e-008"  then pause,end
else
  if msprintf("%g" ,0.0000000345456)   <> "3.45456e-08"   then pause,end
end

if getos() == 'Windows' then
  if msprintf("%g" ,11124234534545)    <> "1.11242e+013"  then pause,end
else
  if msprintf("%g" ,11124234534545)    <> "1.11242e+13"   then pause,end
end

if msprintf("%15g",-35)              <> "            -35" then pause,end
if msprintf("%15g",0.00433)          <> "        0.00433" then pause,end

if getos() == 'Windows' then
  if msprintf("%15g",0.0000000345456)  <> "   3.45456e-008" then pause,end
else
  if msprintf("%15g",0.0000000345456)  <> "    3.45456e-08" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%15g",11124234534545)   <> "   1.11242e+013" then pause,end
else
  if msprintf("%15g",11124234534545)   <> "    1.11242e+13" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.1g",-35.1)            <> "-4e+001"         then pause,end
else
  if msprintf("%.1g",-35.1)            <> "-4e+01"          then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.0g",-35.1)            <> "-4e+001"         then pause,end
else
  if msprintf("%.0g",-35.1)            <> "-4e+01"          then pause,end
end

if getos() == 'Windows' then
  if msprintf("%#.0g",-35.1)           <> "-4.e+001"        then pause,end
else
  if msprintf("%#.0g",-35.1)           <> "-4.e+01"         then pause,end
end

if getos() == 'Windows' then
  if msprintf("%#.0G",-35.1)           <> "-4.E+001"        then pause,end
else
  if msprintf("%#.0G",-35.1)           <> "-4.E+01"         then pause,end
end

if msprintf("%.1g",0.00433)            <> "0.004"           then pause,end

if getos() == 'Windows' then
  if msprintf("%.15g",0.0000000345456) <> "3.45456e-008"    then pause,end
else
  if msprintf("%.15g",0.0000000345456) <> "3.45456e-08"     then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.1g",11124234534545)   <> "1e+013"          then pause,end
else
  if msprintf("%.1g",11124234534545)   <> "1e+13"           then pause,end
end

// format '%e'
// =============================================================================

if getos() == 'Windows' then
  if msprintf("%e",-35)                <> "-3.500000e+001"  then pause,end
else
  if msprintf("%e",-35)                <> "-3.500000e+01"   then pause,end
end

if getos() == 'Windows' then
  if msprintf("%e",35.55)              <> "3.555000e+001"   then pause,end
else
  if msprintf("%e",35.55)              <> "3.555000e+01"    then pause,end
end

if getos() == 'Windows' then
  if msprintf("%+e",35.55)             <> "+3.555000e+001"  then pause,end
else
  if msprintf("%+e",35.55)             <> "+3.555000e+01"   then pause,end
end

if getos() == 'Windows' then
  if msprintf("%e",35.551234567890)    <> "3.555123e+001"   then pause,end
else
  if msprintf("%e",35.551234567890)    <> "3.555123e+01"    then pause,end
end

if getos() == 'Windows' then
  if msprintf("%e",0.00433)            <> "4.330000e-003"   then pause,end
else
  if msprintf("%e",0.00433)            <> "4.330000e-03"    then pause,end
end

if getos() == 'Windows' then
  if msprintf("%e",0.0000000345456)    <> "3.454560e-008"   then pause,end
else
  if msprintf("%e",0.0000000345456)    <> "3.454560e-08"    then pause,end
end

if getos() == 'Windows' then
  if msprintf("%e",11124234534545)     <> "1.112423e+013"   then pause,end
else
  if msprintf("%e",11124234534545)     <> "1.112423e+13"    then pause,end
end

if getos() == 'Windows' then
  if msprintf("%E",11124234534545)     <> "1.112423E+013"   then pause,end
else
  if msprintf("%E",11124234534545)     <> "1.112423E+13"    then pause,end
end

if getos() == 'Windows' then
  if msprintf("%15e",-35)              <> " -3.500000e+001" then pause,end
else
  if msprintf("%15e",-35)              <> "  -3.500000e+01" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%15e",0.00433)          <> "  4.330000e-003" then pause,end
else
  if msprintf("%15e",0.00433)          <> "   4.330000e-03" then pause,end
end


if getos() == 'Windows' then
  if msprintf("%15e",0.0000000345456)  <> "  3.454560e-008" then pause,end
else
  if msprintf("%15e",0.0000000345456)  <> "   3.454560e-08" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%+15e",0.0000000345456) <> " +3.454560e-008" then pause,end
else
  if msprintf("%+15e",0.0000000345456) <> "  +3.454560e-08" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%15e",11124234534545)   <> "  1.112423e+013" then pause,end
else
  if msprintf("%15e",11124234534545)   <> "   1.112423e+13" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.1e",-35)              <> "-3.5e+001"       then pause,end
else
  if msprintf("%.1e",-35)              <> "-3.5e+01"        then pause,end
end

if getos() == 'Windows' then
  if  msprintf("%.0e",-35.1)            <> "-4e+001"        then pause,end
else
  if  msprintf("%.0e",-35.1)            <> "-4e+01"         then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.1e",0.00433)          <> "4.3e-003"        then pause,end
else
  if msprintf("%.1e",0.00433)          <> "4.3e-03"         then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.15e",0.0000000345456) <> "3.454560000000000e-008" then pause,end
else
  if msprintf("%.15e",0.0000000345456) <> "3.454560000000000e-08" then pause,end
end

if getos() == 'Windows' then
  if msprintf("%.1e",11124234534545)   <> "1.1e+013"       then pause,end
else
  if msprintf("%.1e",11124234534545)   <> "1.1e+13"        then pause,end
end

// format '%c'
// =============================================================================

if msprintf("%c","t")     <> "t"          then pause,end
if msprintf("%10c","t")   <> "         t" then pause,end
if msprintf("%10.3c","t") <> "         t" then pause,end
if msprintf("%-10c","t")  <> "t         " then pause,end

// format '%s'
// =============================================================================

if msprintf("%s","text")     <> "text"       then pause,end
if msprintf("%10s","text")   <> "      text" then pause,end
if msprintf("%10.3s","text") <> "       tex" then pause,end
if msprintf("%-10s","text")  <> "text      " then pause,end
if msprintf("%s","t")        <> "t"          then pause,end

// format '%x'
// =============================================================================

if msprintf("%x",123)    <> "7b"         then pause,end
if msprintf("%.10x",123) <> "000000007b" then pause,end
if msprintf("%x",-123)   <> "ffffff85"   then pause,end
if msprintf("%X",-123)   <> "FFFFFF85"   then pause,end
if msprintf("%#.3X",12)  <> "0X00C"      then pause,end

// format '%o'
// =============================================================================
if msprintf("%015o",-12)<>"000037777777764" then pause,end

// Vectorisation
// =============================================================================

nb_row = 10000;

// test 1

A = "row "+string(1:nb_row)';
B = 100*rand(nb_row,3);
C = msprintf("%10s => %08.4f %08.4f %08.4f\n",A,B);

if or(size(C) <> [nb_row,1]) then pause, end

for i=1:nb_row
	if length(C(i))<>40 then pause, end
end

// test 2
B = 100*rand(nb_row,2);
C = 100*rand(nb_row,1);
D = msprintf("%10s => %08.4f %08.4f %08.4f\n",A,B,C);

if or(size(D) <> [nb_row,1]) then pause, end

for i=1:nb_row
	if length(D(i))<>40 then pause, end
end

// test 3
B = 100*rand(nb_row,1);
C = 100*rand(nb_row,1);
D = 100*rand(nb_row,1);
E = msprintf("%10s => %08.4f %08.4f %08.4f\n",A,B,C,D);

if or(size(E) <> [nb_row,1]) then pause, end

for i=1:nb_row
	if length(E(i))<>40 then pause, end
end

// test 4
B = 100*rand(nb_row,1);
if execstr("msprintf(""%10s => %08.4f %08.4f %08.4f\n"",A,B,B);","errcatch")     == 0 then, pause end
if execstr("msprintf(""%10s => %08.4f %08.4f %08.4f\n"",A,B,B,B,B);","errcatch") == 0 then, pause end

// No arg
if execstr("msprintf();","errcatch")     == 0 then, pause, end

// overload: Arg not managed
s=poly(0,"s");
p=1+s+2*s^2;
if execstr("msprintf(""plop"",p);","errcatch") <> 246 then, pause, end


if execstr("msprintf(""%s %s"",""plop"");","errcatch") <> 999 then, pause, end
