#include <cstdio>
#include <iostream>
int fibonacci(int n) {
    if (n == 0 or n == 1) {
        return n;
    }
    return fibonacci(n - 2) + fibonacci(n - 1);
}

// let x: int* = null;

int main() {
    printf("%d", fibonacci(37));
    int x = (true) ? ((true) ? 10 : 20) : 30;
    std::cout << std::endl << x << std::endl;
    return 0;
}