def foo2(param: int): int {
    if (param == 2) {
        return 2;
    }
    return param * foo2(--param);
}

def main(): int {
    print("funciona", foo2(10));
    return 4;
}

def foo(param: int): int {
    if (param == 2) {
        return 2;
    }
    return param * foo(--param);
}