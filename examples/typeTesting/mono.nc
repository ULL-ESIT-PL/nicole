struct Prueba {
    constructor() {}

    destructor {}
}

def foo<T>(): T {
    return T{};
}

def foo2<T>(): int {
    let aa: T = foo<T>();
    return 5;
}

let a: int = foo2<Prueba>();