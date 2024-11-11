/*
IT FAILS BY TE MOMENT
*/

struct A {
    x: int
}

struct B {
    y: A
}

let z: B = B{A{100}};


let aux: A = z.y;

print(aux.x);