let x:int = 10;
def foo<T, Q>(y: T, o:Q) : Q* {
    let z: T = 10;
}

foo<int, int>(x);

struct B {
    x: int
     constructor() {}
    destructor {}
}

struct A<T, Q> extends T {
    constructor(): super() {}
    destructor {}
}

/*

let a: int = 10;

struct A {
    a: int
    constructor() {
       a;
    }

    destructor {}
}

struct B extends A {
    a: int
    constructor(): super() {
       a;
    }

    destructor {}
}

struct C<T, Q> extends W<T> {
    a: Q
    constructor<P>(param: P): super<P>(10, 60) {

    }
}


def foo<T, Q>(a: T*, b: vector<tuple<str***, bool, complexNumber>>): void* {
    let a: T = "hola";
}

let a: int = 0;

struct B {
    a: int
    constructor() {}
    destructor { return 10; }
    method b(): void {
        a;
    }
}

B{};

struct A extends B {
    a: int
    method b(): void {
        a;
    }

    constructor() {}

    destructor {
        a;
    }
}

enum Hola {
    hola,
    adios
}

ss::foo;

*/