def fun(): double {
    return 3.14;
}

auto x = 10;
auto qq = fun() + x;

struct DD {
    x: int
}

struct prueba {
    x: int,
    y: DD
}

auto z = prueba{10, DD{100}};

z.x = 1000;

print(x, "\n")
print(z.x, "\n")
print(qq, "\n")