def foo(x: int): int {
    return x;
}

def foo(x: int, y: int): int {
    return x + y;
}

def foo(x: int, y: int, z: int): int {
    return x + y + z;
}

print(foo(5), " ", foo(5, 5), " ", foo(5, 5, 5), "\n");

auto vv = "hola" + "hola";
auto gg = "hola" + "hola";
print(vv + gg);