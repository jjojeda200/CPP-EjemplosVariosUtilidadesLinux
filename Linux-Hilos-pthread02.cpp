/*
    José Juan Ojeda Granados, 31-12-2021
    pthreads: manejo de múltiples hilos

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

void *funcionThread(void *argValor)
{
    // long numeroHilo = *((long*)argValor);
    // printf("Número de hilo: \e[0;33m %ld \e[0m\n", numeroHilo);
    int numeroHilo = *((int*)argValor);
    printf("\e[0;33m Número de hilo ejecutado:\e[0;37m %d \e[0m\n", numeroHilo);
    pthread_exit(0);

    return 0;
}

int main(int argc, char *argv[])
{
    system("clear");
    
    if (argc != 2)
    {
        printf("%s \e[0;33mUso: Indica el número de hilos\e[0m\n", argv[0]);
        return 1;
    }

    int argValor = atoi(argv[1]); // Convertimos el char de argv en integer
    pthread_t hilos[argValor];    // Definición de variable tipo pthread identificador del hilo
    int errorHilo, hiloNumero;

    printf("\nSe crearan %s hilos\n", argv[1]);

    /*
    int pthread_create(
        pthread_t *restrict thread,
        const pthread_attr_t *restrict attr,
        void *(*start_routine)(void *),
        void *restrict arg);
    */

    for (long i = 0; i < argValor; i++)
    {
        hiloNumero = i;
        printf("\e[0;34mEn Hilo Principal: creando thread\e[0;37m %ld\e[0m\n", i);
        errorHilo = pthread_create(
            &hilos[i],      // Puntero al identificador
            NULL,           // Atributos del hilo, NULL = los por defecto
            &funcionThread, // Rutina a ejecutar al crear el hilo
            &hiloNumero);   // Argumento (único) que se pasa a la rutina
        if (errorHilo != 0)
        {
            printf("Error creando el hilo, código %i \n", errorHilo);
            return -1;
        }
        //sleep(2);
        
        // int pthread_join(pthread_t thread, void **retval);
        // pthread_join(hilos[i], NULL);
    }

    return 0;
}