def fun(x: int): void {
  print("X = ");
  print(x);
  print("\n");
  if (x == 0) {
    print("hola");
    return;
  } else {
    --x;
    fun(x);
  }
}

fun(10);