# Beverly Language

Aspira a ser un lenguaje de programación interpretado, fuertemente tipado, con tipado estático pero con inferencia de tipos, escrito en ANSI C, codificado a fuego lento en noches solitarias y con mucho alcohol.


## Ejemplos:

### Hola mundo:
```rust
print('Hello, world')
```

### Suma de variables:
```rust
let a := 5 // declaracion de tipo implicito .
let b:i32 = 6 // declaracion de tipo explicito.
let c := a + b

print("c:{}", c) // formato de impresion por definir.
```

### Estructura de control FOR:
```rust
// for tipo while
let i := 0 // i32 por defecto.
for (i < 10) {
    print("i:{}\n", i)
}

for (let k:=0; k < 10; k++) {
    print("k:{}\n", i)
}

// foreach
for (index in [0, 1, 2, 3]) {
    print("index:{}\n", index)
}


```



## Actividades

- [ ] Cambiar **BlockStatement** de **IfExpression** por un puntero
- [ ] Reestructurar la función **expression_parser**
- [ ] Programar manejo de errores