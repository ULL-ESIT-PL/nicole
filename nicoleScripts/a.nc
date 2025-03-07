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