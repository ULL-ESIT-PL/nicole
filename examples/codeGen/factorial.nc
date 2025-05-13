def fibonacci(n: int): int {
  if (n == 0 or n == 1) {
    return n;
  }
  return fibonacci(n - 2) + fibonacci(n - 1);
}

def factorial(n: int): int {
  if (n == 0 or n == 1) {
    return 1;
  }
  return n * factorial(n - 1);
}

def main(): int {
    print(factorial(10), "\n");
    print(factorial(5), "\n");
    print(fibonacci(10), "\n");
    return 0;
}