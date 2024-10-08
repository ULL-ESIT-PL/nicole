def fun(x: int): void {
  if (x == 0) {
    print("\n\n\nAitor y alejandro suspenden IA\n\n\n");
    return;
  } 
  fun(--x);
}

fun(10);