def fooInt(): int {
    return 10;
}

def fooDouble(): double {
    return 3.14;
}

def fooFloat(): float {
    return f2.71;
}

def fooChar(): char {
    return 'x';
}

def fooString(): str {
    return "hola";
}

def fooBoolTrue(): bool {
    return true;
}

def fooBoolFalse(): bool {
    return false;
}

def fooIntVar(): int {
    auto x = 10;
    return x;
}

def fooDoubleVar(): double {
    auto x = 3.14;
    return x;
}

def fooFloatVar(): float {
    auto x = f2.71;
    return x;
}

def fooCharVar(): char {
    auto x = 'x';
    return x;
}

def fooStringVar(): str {
    auto x = "hola";
    return x;
}

def fooBoolTrueVar(): bool {
    auto x = true;
    return x;
}

def fooBoolFalseVar(): bool {
    auto x = false;
    return x;
}


print("fooInt() = ", fooInt(), "\n");
print("fooDouble() = ", fooDouble(), "\n");
print("fooFloat() = ", fooFloat(), "\n");
print("fooChar() = ", fooChar(), "\n");
print("fooString() = ", fooString(), "\n");
print("fooBoolTrue() = ", fooBoolTrue(), "\n");
print("fooBoolFalse() = ", fooBoolFalse(), "\n");

print("fooIntVar() = ", fooIntVar(), "\n");
print("fooDoubleVar() = ", fooDoubleVar(), "\n");
print("fooFloatVar() = ", fooFloatVar(), "\n");
print("fooCharVar() = ", fooCharVar(), "\n");
print("fooStringVar() = ", fooStringVar(), "\n");
print("fooBoolTrueVar() = ", fooBoolTrueVar(), "\n");
print("fooBoolFalseVar() = ", fooBoolFalseVar(), "\n");
