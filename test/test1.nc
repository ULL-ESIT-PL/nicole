/*
  dummy script to print a matrix MxN 
  using different loops inside a functions 
  that receives parameters
*/
def fun(m: int, n: int): void {
  while (m > 0) {
    for (let y: int = 0; y < n; ++y) {
      print("0 ");
    }
    print("\n");
    --m;
  }
}

fun(10, 5);