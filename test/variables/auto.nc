

auto x = 10;

struct DD {
    x: int
}

struct prueba {
    x: int,
    y: DD
}

auto z = prueba{10, DD{100}};


print(z.x)