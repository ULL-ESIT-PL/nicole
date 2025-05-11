
struct A {
    y: int
    constructor () {}

    destructor {}
}

struct prueba extends A {
    x: int
    constructor(w: int): super() {
       y := 9999;
       x := w;
       print("dentro de constructor", x + 3, "\n");
    }
    destructor{}
}

auto cc = prueba{22};

cc.x := 12;

print(cc.x, " ", cc.y);

auto p = 10;
auto x = [1, 2, p];
auto y = 0;

print(x[(++y) + 1]);


def foo(): int {
    return ss.x;
}


foo(cc);