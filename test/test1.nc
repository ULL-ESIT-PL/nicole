def fun(x: int): void {
  while (x > 0) {
    for (let y: int = 0; y < 5; ++y) {
      print("0 ");
    }
    print("\n");
    --x;
  }
}

fun(10);