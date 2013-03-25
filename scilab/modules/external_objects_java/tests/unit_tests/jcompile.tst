c = jcompile("Test", ["public class Test {";
       "public int field;";
       "public Test(int n) {";
       "field = n;";
       "}";
       "}";]);
disp(c)
t = c.new(128);
v = jgetfield(t, "field")

// or more easier
junwraprem(t.field)

jremove c t v;
