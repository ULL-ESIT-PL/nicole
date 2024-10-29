/*
auto declaration still not implemented properly
*/

let a: int = 10;
let b: float = f10.0;
let c: double = 10.0;
let d: char = '1';
let e: str = "hola";
let f: bool = false;

print("Doing a declaration\n")
print("a = ", a, "\n");
print("b = ", b, "\n");
print("c = ", c, "\n");
print("d = ", d, "\n");
print("e = ", e, "\n");
print("f = ", f, "\n");
print("---------------\n")

a = -500;
b = f50.8888;
c = 50.8888;
d = '\"';
e = "hello, my name is evian";
f = true;

print("Doing a reassignment\n")
print("a = ", a, "\n");
print("b = ", b, "\n");
print("c = ", c, "\n");
print("d = ", d, "\n");
print("e = ", e, "\n");
print("f = ", f, "\n");
print("---------------\n")

/*
Both fail because a already has been declared
let a: int = 10;
const a: int = 10;
*/
const g: int = 10;
const h: float = f10.0;
const i: double = 10.0;
const j: char = '1';
const k: str = "hola";
const l: bool = false;

print("Doing a const declaration\n")
print("g = ", g, "\n");
print("h = ", h, "\n");
print("i = ", i, "\n");
print("j = ", j, "\n");
print("k = ", k, "\n");
print("l = ", l, "\n");
print("---------------\n")

/*
Will fail because its const
g = 10000;
*/

let z: int = a;

print("Doing a variable call (already doing in every print) for an assignment\n");
print("z = a = ", z, "\n");