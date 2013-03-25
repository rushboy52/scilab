jimport java.lang.Double;
d = Double.new("1.23456");
assert_checktrue(jexists(d))
jremove d;
assert_checkfalse(jexists(d))

