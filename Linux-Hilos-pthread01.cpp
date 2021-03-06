/*
    José Juan Ojeda Granados, 30-12-2021
    pthreads: manejo de hilos, crear uno, escribir en un fichero y esperar finalización

    La función pthread_create() inicia un nuevo hilo en la llamada proceso. El nuevo hilo comienza la ejecución invocando funcionThread(); arg se pasa como el único argumento de
    funcionThread().
    La función pthread_join espera la terminación del hilo. En el puntero void **retval se almacena el estado de salida devuelto del hilo. Si no se desea recuperar el código de salida del hilo esperado, se debe pasar el valor NULL como segundo argumento.

    https://man7.org/linux/man-pages/man7/pthreads.7.html

    Link con "-pthread" Compilar:
    gcc Linux-Hilos-pthread01.cpp -pthread -o Linux-Hilos-pthread
*/
#include <pthread.h>
#include <fstream>
//#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *funcionThread(void *argValor)
{
    int numeroLineas = *((int *)argValor);
    int descriptorArchivo;
    char bufer[] = "Nueva línea a añadir al archivo de prueba\n";

    printf("\e[0;33mID de hilo Uno:\e[0;37m %u\n\e[0m", (unsigned int)pthread_self());

    for (int i = 0; i < numeroLineas; i++)
    {
        descriptorArchivo = open("./prueba.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
        write(descriptorArchivo, bufer, sizeof(bufer) - 1);
        if (descriptorArchivo == -1)
        {
            printf("[Error] %d: %s \n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(descriptorArchivo);
        sleep(60);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    system("clear");

    if (argc != 2)
    {
        printf("%s \e[0;33mUso: Indica el número de lineas a añadir al archivo\e[0m\n", argv[0]);
        return 1;
    }

    pthread_t hiloUno;            // Definición de variable tipo pthread identificador del hilo
    int argValor = atoi(argv[1]); // Convertimos el char de argv en integer
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
        &argValor);     // Argumento (único) que se pasa a la rutina

    if (errorHilo != 0)
    {
        printf("Error creando el hilo, código %i \n", errorHilo);
        return -1;
    }

    int retorno;
    void *retval;
    retorno = pthread_join(hiloUno, &retval);
    if (retval == PTHREAD_CANCELED)
        printf("El hilo se ha canelado");
    else
        printf("\e[0;33mValor retornado del hilo\e[0;37m %lu \e[0m\n", (long)retval);

    // int pthread_join(pthread_t thread, void **retval);
    // pthread_join(hiloUno, NULL);

    return 0;
}