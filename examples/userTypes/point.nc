/*
still methods no implemented, neither struct composition
*/

struct point3D {
    x: int,
    y: int,
    z: int
}

struct pointD {
    x: int,
    y: int,
    z: int
}

let p: point3D = point3D{0, 0, 0};

print(p.x, " ", p.y, " ", p.z, "\n");

p.x = 1;
p.y = 2;
p.z = -10000;

print(p.x, " ", p.y, " ", p.z, "\n");