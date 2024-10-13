#include <cstdio>
int fibonacci(int n) {
    if (n == 0 or n == 1) {
        return n;
    }
    return fibonacci(n - 2) + fibonacci(n - 1);
}

// let x: int* = null;

int main() {
    printf("%d", fibonacci(37));
    return 0;
}