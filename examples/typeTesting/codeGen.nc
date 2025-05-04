print(1, "hola", null);
print("\n", "hola", null);
auto x = "10";
let y: bool = false;

print(x);

if (false) {
    print(x);
} else if (false) {
    print("else iff\n");
} else if (false) {
    print("second else iff\n");
}   else {
    print(y);
}

print((false) ? "hola" : "adios");


enum AA {
    a,
    b
}

let ww: AA = AA::b;

print(AA::b);

auto qq = 3;

print(qq);


switch(qq) {
    case 1: {
        print("primer case");
    }
    case 2: {
        print("segundo case");
    }
    default {
        print("default");
    }
}


while (true) {
    print("hola\n");
    if (true) {
    pass;
    }
    print("adios\n");
}