struct Prueba {
    constructor() {}

    destructor {}
}

struct PPPP extends Prueba {
    constructor(): super() {}

    destructor {}
}

def foo<T>(): T {
    return T{};
}

def foo<T, Q>(): Q {
    return 5;
}

def foo2<T>(): int {
    let aa: T = foo<T>();
    return 5;
}

let a: int = foo2<Prueba>();


foo2<int>();
foo<int, str>();


def ggggggggg_<T>(fgf: T): T {
    return T{};
}


ggggggggg_<Prueba>(Prueba{});

let kkkkkk: Prueba* = new PPPP{}; 




struct QQQ {
    constructor() {}

    virtual destructor {}

 method eeee(): void {}
}