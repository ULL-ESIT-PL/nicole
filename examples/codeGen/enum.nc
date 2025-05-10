enum prueba {
    a, 
    b, 
    c
}

print(prueba::a, " ", prueba::b, " ", prueba::c, "\n");

auto a = prueba::a;

print(a, "\n");

let b: prueba = prueba::b;

print(b, "\n");

b := a;

print(b, "\n");

b := prueba::c;

print(b, "\n");