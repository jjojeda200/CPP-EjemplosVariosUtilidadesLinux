/*
    José Juan Ojeda Granados, 30-12-2021
    pthreads: manejo de hilos, crear un hilo y ejecutar una terminal y no esperar finalización.

    La función pthread_create() inicia un nuevo hilo en la llamada proceso. El nuevo hilo comienza la ejecución invocando funcion(); arg se pasa como el único argumento de
    funcion().

    https://man7.org/linux/man-pages/man7/pthreads.7.html

    Link con "-pthread" Compilar:
    gcc Linux-Hilos-pthread00.cpp -pthread -o Linux-Hilos-pthread
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *funcion(void *arg)
{
    system("xfce4-terminal");
    printf("ID de hilo Uno: %u\n", (unsigned int)pthread_self());
    return 0;
}

int main()
{
    pthread_t hiloUno;
    /*
    int pthread_create(
        pthread_t *restrict thread,
        const pthread_attr_t *restrict attr,
        void *(*start_routine)(void *),
        void *restrict arg);
    */

    pthread_create(&hiloUno, NULL, &funcion, NULL);

    // pthread_join(nombreDeHilo, NULL); ejecuta el hilo en paralelo al principal y espera a que termine antes de continuar con la siguiente instrucción del main.
    pthread_join(hiloUno, NULL);

    // La función pthread_self() devuelve el ID del hilo en el que se invoca. provoca continuar con la siguiente instrucción del main.
    printf("ID de hilo original:%u\n", (unsigned int)pthread_self());
    return 0;
}