/*
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

def eApprox(n: int): double {
  let result: double = 0.0;
  for (let i: int = 0; i < n; ++i) {
    result = result + (1.0 / factorial(i));
  }
  return result;
}

print("Finbonacci: ", fibonacci(37), 
      "\nFactorial: ", factorial(10), 
      "\nE approximation: ", eApprox(30), "\n");
*/

let x: int = 0;

switch (x) {
  case 0: {
    print("c++");
  }
  case 1: {
    print("adios");
  }
  default: {
    print("hola");
  }
}