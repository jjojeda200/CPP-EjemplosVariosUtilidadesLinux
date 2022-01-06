/*
    José Juan Ojeda Granados, 16-12-2021
    Llamadas de aplicación con argumentos...
*/
// #include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) // Argumentos de entrada, prueba de concepto
{
    system("clear");

    printf("Soy el ejecutable \e[0;33m%s\e[0m, con PID \e[0;33m%i\e[0m\n",argv[0], getpid());

    printf("Los argumentos se trasladan en un vector array \e[0;34m\"*argv[]\"\e[0m de n+1 elementos,\n");
    printf("siendo el primer argumento del array el propio programa y el ultimo un cero (0),\n");
    printf("se determinan su número +1 y almacena en \e[0;34m\"argc\"\e[0m\n");
    int aux{};
    for (int i = 0; i < argc; i++)
    {
        printf("Número de argumento: %d, argumento: \e[0;33m  %s \e[0m\n", i, argv[i]);
        aux++;
    }
    printf("Número de argumentos pasados al invocar el programa: \e[0;34m %d \e[0m\n", --aux);
    printf("El último argumento del vector por defecto siempre es 0:\e[0;33m %s \e[0m\n", argv[argc]);

    return 0;
}