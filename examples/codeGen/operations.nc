/* Números enteros */
print("1 + 1 =", 1 + 1, "\n");
print("1 - 1 =", 1 - 1, "\n");
print("3 * 7 =", 3 * 7, "\n");
print("10 / 2 =", 10 / 2, "\n");
print("10 % 3 =", 10 % 3, "\n");
print("(1 + 2) * 3 =", (1 + 2) * 3, "\n");
print("1 < 2 =", 1 < 2, "\n");
print("1 <= 1 =", 1 <= 1, "\n");
print("1 > 0 =", 1 > 0, "\n");
print("1 >= 1 =", 1 >= 1, "\n");
print("1 == 1 =", 1 == 1, "\n");
print("1 != 2 =", 1 != 2, "\n");

print("\n");  /* Separador */

/* Números decimales double*/
print("1.5 + 2.3 = ", 1.5 + 2.3, "\n");
print("5.0 - 2.0 = ", 5.0 - 2.0, "\n");
print("2.5 * 4.0 = ", 2.5 * 4.0, "\n");
print("9.0 / 3.0 = ", 9.0 / 3.0, "\n");
print("1.0 < 2.0 = ", 1.0 < 2.0, "\n");
print("1.0 > 2.0 = ", 1.0 > 2.0, "\n");
print("3.3 == 3.3 = ", 3.3 == 3.3, "\n");

/* Números decimales float*/
print("f1.5 + f2.3 = ", f1.5 + f2.3, "\n");
print("f5.0 - f2.0 = ", f5.0 - f2.0, "\n");
print("f2.5 * f4.0 = ", f2.5 * f4.0, "\n");
print("f9.0 / f3.0 = ", f9.0 / f3.0, "\n");
print("f1.0 < f2.0 = ", f1.0 < f2.0, "\n");
print("f1.0 > f2.0 = ", f1.0 > f2.0, "\n");
print("f3.3 == f3.3 = ", f3.3 == f3.3, "\n");

print("\n");  /* Separador */

/* Caracteres */
print("\'a\' == \'a\' = ", 'a' == 'a', "\n");
print("\'a\' != \'b\' = ", 'a' != 'b', "\n");
print("\'a\' < \'b\' = ", 'a' < 'b', "\n");
print("\'z\' > 'x' = ", 'z' > 'x', "\n");

print("\n");  /* Separador */

/* Strings */
print("\"hola\" + \" mundo\" = ", "hola" + " mundo", "\n");
print("\"abc\" == \"abc\" = ", "abc" == "abc", "\n");
print("\"abc\" != \"def\" = ", "abc" != "def", "\n");
print("\"abc\" < \"def\" = ", "abc" < "def", "\n");
print("\"zzz\" > \"aa\" = ", "zzz" > "aaa", "\n");

print("\n");  /* Separador */

/* Booleanos */
print("true and false =", true and false, "\n");
print("true or false =", true or false, "\n");
print("not true =", !true, "\n");
print("1 == 1 and 2 > 1 =", 1 == 1 and 2 > 1, "\n");
print("3 < 2 or 4 == 4 =", 3 < 2 or 4 == 4, "\n");


/* Con variables */
/* Números enteros */
auto a = 1;
auto b = 3;
auto c = 7;
auto d = 10;
auto e = 2;
auto f = 0;
print("1 + 1 = ", a + a, "\n");
print("1 - 1 = ", a - a, "\n");
print("3 * 7 = ", b * c, "\n");
print("10 / 2 = ", d / e, "\n");
print("10 % 3 = ", d % b, "\n");
print("(1 + 2) * 3 = ", (a + b) * c, "\n");
print("1 < 2 = ", a < b, "\n");
print("1 <= 1 = ", a <= a, "\n");
print("1 > 0 = ", a > f, "\n");
print("1 >= 1 = ", a >= a, "\n");
print("1 == 1 = ", a == a, "\n");
print("1 != 2 = ", a != e, "\n");

print("\n");

/* Números decimales double */
auto d1 = 1.5;
auto d2 = 2.3;
auto d3 = 5.0;
auto d4 = 2.0;
auto d5 = 2.5;
auto d6 = 4.0;
auto d7 = 9.0;
auto d8 = 3.0;
auto d9 = 3.3;
print("1.5 + 2.3 = ", d1 + d2, "\n");
print("5.0 - 2.0 = ", d3 - d4, "\n");
print("2.5 * 4.0 = ", d5 * d6, "\n");
print("9.0 / 3.0 = ", d7 / d8, "\n");
print("1.0 < 2.0 = ", d4 < d2, "\n");
print("1.0 > 2.0 = ", d4 > d2, "\n");
print("3.3 == 3.3 = ", d9 == d9, "\n");

print("\n");

/* Números decimales float */
auto f1 = f1.5;
auto f2 = f2.3;
auto f3 = f5.0;
auto f4 = f2.0;
auto f5 = f4.0;
auto f6 = f9.0;
auto f7 = f3.0;
auto f8 = f3.3;
print("f1.5 + f2.3 = ", f1 + f2, "\n");
print("f5.0 - f2.0 = ", f3 - f4, "\n");
print("f2.5 * f4.0 = ", f1 * f5, "\n");
print("f9.0 / f3.0 = ", f6 / f7, "\n");
print("f1.0 < f2.0 = ", f4 < f2, "\n");
print("f1.0 > f2.0 = ", f4 > f2, "\n");
print("f3.3 == f3.3 = ", f8 == f8, "\n");

print("\n");

/* Caracteres */
auto ch1 = 'a';
auto ch2 = 'b';
auto ch3 = 'z';
auto ch4 = 'x';
print("'a' == 'a' = ", ch1 == ch1, "\n");
print("'a' != 'b' = ", ch1 != ch2, "\n");
print("'a' < 'b' = ", ch1 < ch2, "\n");
print("'z' > 'x' = ", ch3 > ch4, "\n");

print("\n");

/* Strings */
auto s1 = "hola";
auto s2 = " mundo";
auto s3 = "abc";
auto s4 = "def";
auto s5 = "zzz";
auto s6 = "aaa";
print("\"hola\" + \" mundo\" = ", s1 + s2, "\n");
print("\"abc\" == \"abc\" = ", s3 == s3, "\n");
print("\"abc\" != \"def\" = ", s3 != s4, "\n");
print("\"abc\" < \"def\" = ", s3 < s4, "\n");
print("\"zzz\" > \"aaa\" = ", s5 > s6, "\n");

print("\n");

/* Booleanos */
auto t = true;
auto n = false;
print("true and false = ", t and n, "\n");
print("true or false = ", t or n, "\n");
print("not true = ", !t, "\n");
print("1 == 1 and 2 > 1 = ", a == a and e > a, "\n");
print("3 < 2 or 4 == 4 = ", b < e or c == c, "\n");


/* increment decrement */

auto increment = 10;
++increment;
print(++increment, "\n");

auto decrement = 10;
--decrement;
print(--decrement, "\n");