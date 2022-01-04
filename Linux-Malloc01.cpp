/*
    José Juan Ojeda Granados, 04-01-2022
    Funciones malloc y free. Asignación dinámica de memoria.

    Asignación en tiempo de ejecución, en memoria HEAP

    https://man7.org/linux/man-pages/man3/free.3.html

    https://man7.org/linux/man-pages/man3/malloc_info.3.html
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void rellenaMemoria(char *pun, unsigned int numBytes)
{
    for (int i = 0; i < numBytes; i++)
    {
        pun[i] = 'X';
    }
    pun[numBytes] = '\0';
    printf("Cadena:\e[0;33m %s \e[0m\n", pun);
}

int main(int argc, char *argv[])
{
    system("clear");

    unsigned int numeroBytes;
    char *puntero = NULL;

    if (argc != 2)
    {
        printf("%s \e[0;33mUso: Indica el número de bytes a reservar\e[0m\n", argv[0]);
        return 1;
    }
    numeroBytes = atoi(argv[1]);
    printf("\e[0;34mNúmero de bytes indicados:\e[0;33m %d \e[0m\n", numeroBytes);

    // void *malloc(size_t size);
    puntero = (char *)malloc(numeroBytes);
    if (puntero == NULL)
    {
        printf("Se ha producido un error");
        return 1;
    }
    else
    {
        rellenaMemoria(puntero, numeroBytes);
    }
    // void free(void *ptr);
    free(puntero);

    return 0;
}