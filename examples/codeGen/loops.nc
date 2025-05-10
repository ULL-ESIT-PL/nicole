/* Using stop */
while (true) {
    print("hola\n");
    stop;
}

auto i = 0;
while (i < 5) {
    if (i == 3) {
       ++i;
        pass;
    }
    print(i, " ");
    ++i;
}


do {
    print("\nhola\n");
}   while (i != 5);


for (auto j = 0; j < 5; ++j) {
    for (auto k = 0; k < 5; ++k) {
        print(k, " ");
    }
    print("\n");
}

print("\n");

auto w = 0;
auto z = 0;
auto y = 5;

for (; w < y;) {
    if (w == 3) {
        ++w;
        pass;
    } else if (w == 4) {
        stop;
    } 
    for (; z < y;) {
        print(z, " ");
        ++z;
    }
    print("\n");
    ++w;
    z := 0;
}