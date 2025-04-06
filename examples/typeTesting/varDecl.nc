auto x = 10 + 4;

let y: int = 10; /* missing conversion between types */

/*
Ambos fallan de momento
let a: int* = new int{3};
let b: int** = new int*{new int{6}};
*/

struct Hola {
    constructor() {}
    destructor {}
}

struct Adios extends Hola {
    constructor(): super() {}
    destructor {}
}

let z: Hola = Hola{};

let w: Hola* = new Hola{};

enum Enu {
    a
}

let s: Enu = Enu::a;