Cómo evitar problemas con la asignación dinámica de memoria en C

by Francisco Charte.
https://fcharte.com/tutoriales/20070331-AsignacionMemoriaC/

Todos los que programamos con C/C++, incluso los que nos consideramos experimentados, solemos emplear una significativa cantidad de tiempo depurando errores siempre relacionados con el mismo tema: la asignación dinámica de memoria. Siguiendo una serie de reglas básicas podemos reducir apreciablemente este tipo de fallos:

    Antes de usar un puntero comprobar que no es NULL. Introducir un if(q) usa_q_como_sea; nos ahorrará muchos disgustos.

    Asignar el valor NULL a todo puntero sin inicializar. Acostumbrarse a escribir char* q = NULL es una buena costumbre.

    Tras asignar memoria dinámicamente, inicializarla de inmediato. Tras un q = malloc(sizeof(X)) siempre un memset(q,0,sizeof(X)), así evitaremos usar la basura que hay en la memoria sin inicializar.

    Usar siempre strncpy() y strncat() al operar sobre cadenas asignadas dinámicamente, así evitaremos escribir más allá del tamaño del bloque, en zonas de memoria que no nos pertenecen.

    No calcular nunca a mano el tamaño de los operandos, en su lugar recurrir siempre al operador sizeof(). El tamaño de los tipos básicos y la alineación de los campos en las estructuras alteran el espacio que se ocupa.

    Al trabajar con cadenas, no olvidar que si hemos asignado N bytes dinámicamente, sólo podremos almacenar N-1 caracteres. Obvio, pero fuente de muchos quebraderos de cabeza.

    Antes de asignar a un puntero no nulo otra dirección, no olvidar liberar la memoria previamente asignada. La construcción if(q) free(q) ha de ser una costumbre antes de reutilizar una variable puntero.

    Cada free(q) irá siempre seguido de un q = NULL, así evitaremos acceder a bloques de memoria que ya hemos liberado.

    Moldear siempre los punteros al tipo correcto, de lo contrario la aritmética de punteros no funcionará. Una operación del tipo q++ nos llevará a una dirección u otra dependiendo del tipo de q.
    
    Antes de usar un puntero comprobar que no es NULL. Esta regla, primera y última, es fundamental y nunca suficientemente repetida. Comprobar siempre que un puntero no es NULL antes de leer o escribir con él.

Espero que estas indicaciones puedan ser de alguna utilidad a los que programan diariamente utilizando el lenguaje C.