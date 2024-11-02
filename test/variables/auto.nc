

auto x = 10;

struct prueba {
    x: int,
    y: str
}

auto z = prueba{10, "hola"};

z.x = 100;

print(z.x)