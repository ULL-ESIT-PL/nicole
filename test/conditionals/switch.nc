let x: int = 4;
const y: int = 4;
/*
switch works with int, char, bool
also a case can be a const variable
*/
switch (x) {
    case 10: {
        print("Another feature if this gets printed");
    }
    case y: {
        print("Hellooooooooooooooooo\n");
    }
}

--x;

switch (x) {
    case 10: {
        print("Another feature if this gets printed");
    }
    case y: {
        print("Hellooooooooooooooooo\n");
    }
    /*
        If default is not the last one a parsing error will abort the compiling
    */
    default: {
        print("As you can see I'm optional\n");
    }
}