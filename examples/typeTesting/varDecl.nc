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

struct Prueba<T> {
    constructor<Q>() {
        let qqqq: Q = 10;
        if (!qqqq) {}
    }
    destructor {}

    method toString(): str { return ""; }
}

Prueba<Enu***>{};

Enu{Enu::a};

const wewe: int* = new 5;
if (null != wewe) {}

const xxxx: int = 10;

/*
xxxx := 10;
*/
if (!1) {
    return ;
}

return;

print(Prueba<int>{});


def fooooooo (): void { 
    if (true) {
        return ;
    }
 }



 struct Madrid {
    x: int
    constructor() {}
    destructor {}
}

Madrid{}.x + 1;


return;


struct T {
    constructor() {}
    destructor {}
}

def lll<T>(): void {
    let cccc: T = T{};
}

def vec(): vector<int> {
    return [10];
}

vec()[1];

Prueba<int>{}.toString()[1];


lll<int>();

def hhh(kkl: int): int {
    return 5;
}

hhh(1);


let zzzzzz: Hola*  = new Adios{};