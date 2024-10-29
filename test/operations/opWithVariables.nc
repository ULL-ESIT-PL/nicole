/*
Still string concatenation not supported
*/
let a: int = 10;
let b: float = f10.0;
let c: double = 10.0;
let d: char = '1';
let e: str = "hola";
let f: bool = false;

print(a * a, "\n");
print(a + a, "\n");
print(a - a, "\n");
print(a / a, "\n");
print(!(a == a), "\n");
print(c * 2 / (c + 1), "\n");
print(a < a, "\n");
print(a > a, "\n");
print(a <= a, "\n");
print(a >= a, "\n");
print(a != a, "\n");
print(a % 4, "\n");

a += 10;
a -= 5;
a *= 2;
a /= 3;

print(a, "\n");

/*
Increment and decrement are only suported for integers
*/
++a;
print(a, "\n");
--a;
print(a, "\n");