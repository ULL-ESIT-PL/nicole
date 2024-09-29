struct point {
  let x: int = 0;
  let y: int = 0;
  let z: int = 0;
}

let x: int = 10;
let y: int = x + 10;
if (true) {
    let z: int = 10;
    print(x + z);
}
print(y);
print(x + 10 + y);