if (true) {
    print("primer if\n");
}

if (false) {
    print("primer if\n");
} else if (true) {
    print("else if\n");
}

if (false) {
    print("primer if\n");
} else if (false) {
    print("else if\n");
} else {
    print("else \n");
}

if (true) {
    if (false) {
        print("primer if\n");
    } else if (true) {
        print("else if\n");
    }
}


auto condition = 10;

switch (condition) {
    case 4: {
        print("primer case\n");
    }
    case 10: {
        print("segundo case\n");
    }
    default {
        print("default case\n");
    }
}

enum prueba {
    a, b, c
}

auto condition2 = prueba::b;

switch (condition2) {
    case prueba::a: {
        print("primer case\n");
    }
    case prueba::b: {
        print("segundo case\n");
    }
    default {
        print("default case\n");
    }
}

auto condition3 = prueba::c;

switch (condition3) {
    case prueba::a: {
        print("primer case\n");
    }
    case prueba::b: {
        print("segundo case\n");
    }
    default {
        return;
        print("default case\n");
    }
}