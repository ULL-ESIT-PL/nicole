/*
Functions do not inherit scopes
If/else/do while/for/while inherit outer scope
*/

if (true) {
    print("Madrid > Barcelona\n");
}

if (false) {
    print("If I get printed we call this a feature, if not, it is working correctly\n");
} else {
    print("Just forgot to put WHERE in the production database\n");
}

let x: bool = true;

if (x) {
    print("Will Nicole replace C??? \n");
}

let y: int = 10;

if (y < 100) {
    print("Nicole is basically a sintax Frankenstein\n");
}

if (!(y > 100 or y == -1)) {
    print("Takes inspiration from c++, typeScript and python\n");
}

if (true) {
    let dummy: str = "World Hello\n";
    if (true) {
        let err: str = "If I tell you how to leave VIM, I must kill you\n";
        print(dummy)
    }
    /*
      compile error because is not declard in this scope
      print(err);
    */
}

/*
  compile error because is not declard in this scope
  print(dummy);
*/

/*
Ternary operator
*/

let ternary: str = (45 < 10) ? "PI = E = 3\n" : "Nicole is already at the top of Tiobe Index";
print((45 > 10) ? "The more you use a debugger the tastier are print statements\n" : "Another feature if this gets printed\n");