/*
    José Juan Ojeda Granados, 31-12-2021
    pthreads: manejo de hilos, crear uno, escribir en un fichero y esperar finalización

    La función pthread_create () inicia un nuevo hilo en la llamada proceso. El nuevo hilo comienza la ejecución invocando start_routine(); arg se pasa como el único argumento de
        start_routine ().

    https://man7.org/linux/man-pages/man7/pthreads.7.html

    Link con "-pthread" Compilar:
    gcc Linux-Hilos-pthread02.cpp -pthread -o Linux-Hilos-pthread
*/
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


