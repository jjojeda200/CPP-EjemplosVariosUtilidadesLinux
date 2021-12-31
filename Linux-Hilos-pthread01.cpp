/*
    José Juan Ojeda Granados, 30-12-2021
    pthreads: manejo de hilos, crear uno, escribir en un fichero y esperar finalización

    La función pthread_create() inicia un nuevo hilo en la llamada proceso. El nuevo hilo comienza la ejecución invocando start_routine(); arg se pasa como el único argumento de
    start_routine ().

    https://man7.org/linux/man-pages/man7/pthreads.7.html

    Link con "-pthread" Compilar:
    gcc Linux-Hilos-pthread00.cpp -pthread -o Linux-Hilos-pthread
*/
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// int pthread_join(pthread_t thread, void **retval);


void *funcionThread(void *argValor)
{
    
    system("xfce4-terminal");
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t hiloUno; // Definicion de variable tipo pthread identificador del hilo
    int argValor;
    int errorHilo;

    /*
    int pthread_create(
        pthread_t *restrict thread,
        const pthread_attr_t *restrict attr,
        void *(*start_routine)(void *),
        void *restrict arg);
    */
     errorHilo = pthread_create(
                        &hiloUno,       // Puntero al identificador
                        NULL,           // Atributos del hilo, NULL = los por defecto
                        &funcionThread, // Rutina a ejecutar al crear el hilo
                        &argValor);     // Argumento (unico) que se pasa a la rutina

    if (errorHilo != 0)
    {
        printf("Error creando el hilo \n");
        return -1;
    }

    return 0;
}