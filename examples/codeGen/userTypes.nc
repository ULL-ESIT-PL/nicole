struct A {
    y: int
    constructor () {}

    destructor {}
}

struct prueba extends A {
    x: int
    constructor(w: int): super() {
        /*
        print(w, "------");
      */
       y := 9999;
       x := w;
       print("dentro de constructor", x + 3, "\n");
    }
    destructor{}
}

auto cc = prueba{22};

cc.x := 12;

print(cc.x, " ", cc.y);
