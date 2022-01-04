/*
    José Juan Ojeda Granados, 04-01-2022
    Funciones malloc y free. Asignación dinámica de memoria.

    Asignación en tiempo de ejecución, en memoria HEAP

    https://man7.org/linux/man-pages/man3/free.3.html

    https://man7.org/linux/man-pages/man3/malloc_info.3.html
*/
#include <stdio.h>  /* printf, scanf, NULL */
#include <stdlib.h> /* malloc, free, rand */

// Ejemplo malloc: generador de cadena de caracteres aleatoria
int main()
{
    system("clear");

    int longitud;
    char *bufer;

    printf("\e[0;33m¿Indica la longitud de la cadena de caracteres?:\e[0m ");
    scanf("%d", &longitud);

    bufer = (char *)malloc(longitud + 1);
    if (bufer == NULL)
        exit(1);

    for (int n = 0; n < longitud; n++)
        bufer[n] = rand() % 26 + 'a';
    bufer[longitud] = '\0';

    printf("Cadena aleatoria:\e[0;33m %s \e[0m\n", bufer);
    free(bufer);

    return 0;
}