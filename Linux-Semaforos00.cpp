/*

    José Juan Ojeda Granados, 03-01-2022
    Programar con semáforos POSIX

    Ejemplo en el que dos hilos (threads) acceden a un "recurso compartido" y se producen errores, por lo que es necesario implementar semáforos para proteger la "sección crítica":

    int sem_init(sem_t *sem, int pshared, unsigned int value);
        Función:
            Crea un semáforo e inicializa su valor. Se debe inicializar un semáforo sin nombre antes de ser utilizado.
        Parámetro:
            sem: la dirección del semáforo.
            pshared: si es igual a 0, El semáforo se comparte entre hilos (de uso común); si no no es igual a 0, Los semáforos se comparten entre procesos.
            valor: el valor inicial del semáforo.

        Valor de retorno:
            Éxito:      0
            Fallido:   -1

    int sem_destroy(sem_t *sem);
        Función:
            Eliminar el semáforo identificado por sem.
        Parámetro:
            sem: dirección del semáforo.
        Valor de retorno:
            Éxito:      0
            Fallido:   -1

    int sem_wait(sem_t *sem);
        Función:
            Resta el valor del semáforo 1. Antes de la operación, verifca si el valor del semáforo (sem) es 0, Si el semáforo es 0, esta función se bloqueará hasta que el semáforo sea mayor que 0.
        Parámetro:
            sem: la dirección del semáforo.
        Valor de retorno:
            Éxito:      0
            Fallido:   -1

    int sem_post(sem_t *sem);
        Función:
            Agregar el valor del semáforo 1 y envía una señal para despertar el hilo de espera sem_wait()
        Parámetro:
            sem: la dirección del semáforo.
        Valor de retorno:
            Éxito:      0
            Fallido:   -1

    int sem_getvalue(sem_t *sem, int *sval);
        Función:
            Obtener el valor del semáforo identificado por sem y guárdelo en sval.
        Parámetro:
            sem: dirección del semáforo.
            sval: la dirección para guardar el valor del semáforo.
        Valor de retorno:
            Éxito:      0
            Fallido:   -1

    https://man7.org/linux/man-pages/man3/sem_init.3.html
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define loop 10000

sem_t semUno, semDos;   // Define dos semáforos
static int contador = 0;

void *pthreadHiloUno(void *arg) // Este hilo cambia el valor del caracter ch
{
    printf("\e[0;33mID de hilo uno:\e[0;34m%u\e[0m\n", (unsigned int)pthread_self());
    for (int i = 0; i < loop; i++)
    {
        sem_wait(&semUno);
        contador++;
        sem_post(&semDos);
    }
    pthread_exit(0);
    return 0;
}

void *pthreadHiloDos(void *arg) // Este hilo imprime el valor de ch
{
     printf("\e[0;33mID de hilo dos:\e[0;34m%u\e[0m\n", (unsigned int)pthread_self());
    for (int i = 0; i < loop; i++)
    {
        sem_wait(&semDos);
        contador--;
        sem_post(&semUno);
    }
    pthread_exit(0);
    return 0;

}

int main(int argc, char *argv[])
{
    system("clear");

    pthread_t hiloUno, hiloDos;

    // La función pthread_self() devuelve el ID del hilo en el que se invoca. provoca continuar con la siguiente instrucción del main.
    printf("\e[0;33mID de hilo original:\e[0;34m%u\e[0m\n", (unsigned int)pthread_self()); 

    // Mientras los semaforos este desabilitados, el resultado nunca sera 0, que es lo que cabe esperar de la ejecución de las dos funciones.
     sem_init(&semUno, 0, 0); // Inicializa el semáforo
     sem_init(&semDos, 0, 1);

    /*
    int pthread_create(
        pthread_t *restrict thread,             // Puntero al identificador
        const pthread_attr_t *restrict attr,    // Atributos del hilo, NULL = los por defecto
        void *(*start_routine)(void *),         // Rutina a ejecutar al crear el hilo
        void *restrict arg);                    // Argumento (único) que se pasa a la rutina
    */
    pthread_create(&hiloUno, NULL, pthreadHiloUno, NULL);
    pthread_create(&hiloDos, NULL, pthreadHiloDos, NULL);

    // pthread_join(nombreDeHilo, NULL); ejecuta el hilo en paralelo al principal y espera a que termine antes de continuar con la siguiente instrucción del main.
    pthread_join(hiloUno, NULL);
    pthread_join(hiloDos, NULL);

    printf("Valor contafor %d\n", contador);

    return 0;
}