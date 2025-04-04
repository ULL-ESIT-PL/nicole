class A extends B {
private:
    attr x: str = "hola";
    
    constructor(x, y_): super(y_) {
        this.x = x;
    }

protected:
    attr y: str = "adios";

public:
    method override adios(): void {
        this.x = "adios";
    }
}