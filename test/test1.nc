let x: int = 10;
/*
  %x = alloca i32, align 4
  store i32 10, i32* %x, align 4
*/
x = 100 + 5;
/*
  store i32 105, i32* %x, align 4
*/
let y: int = x;

y;