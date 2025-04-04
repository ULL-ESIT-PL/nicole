/*
Also loops accept nested, function calls, if...
*/

let x: int = 10;
let y: int = 5;

for (let i: int = 0; i < x; ++i) {
  let j: int = 0;
  while (j < y) {
    print("* ");
    ++j;
  }
  print("\n");
}

let z: int = 0;
do {
    print("HTML IS A LANGUAGE!!!!!\n");
    ++z;
} while (z < 3)