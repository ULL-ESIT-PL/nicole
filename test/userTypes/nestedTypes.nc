struct DD {
    x: int
}

struct prueba {
    x: int,
    y: DD
}

let z: prueba = prueba{10, DD{100}};


let aux: DD = z.y;

print(aux.x)