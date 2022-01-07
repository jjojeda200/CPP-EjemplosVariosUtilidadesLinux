/*

    José Juan Ojeda Granados, 07-01-2022
    Programar con mutex POSIX - complemento de hilos

    La sincronización de subprocesos se define como un mecanismo que garantiza que dos o más procesos o subprocesos simultáneos no ejecuten simultáneamente algún segmento de programa en particular conocido como sección crítica. El acceso de los procesos a la sección crítica se controla mediante técnicas de sincronización. Cuando un hilo comienza a ejecutar la sección crítica (un segmento serializado del programa), el otro hilo debe esperar hasta que finalice el primer hilo. Si no se aplican las técnicas de sincronización adecuadas, puede causar una condición de carrera en la que los valores de las variables pueden ser impredecibles y variar según los tiempos de los cambios de contexto de los procesos o subprocesos.

    Ejemplo en el que dos hilos (threads) acceden a un "recurso compartido" y se producen errores, por lo que es necesario implementar pthread_mutex_lock para proteger la "sección crítica"

    https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html

    int pthread_mutex_lock(pthread_mutex_t *mutex);
    int pthread_mutex_trylock(pthread_mutex_t *mutex);
    int pthread_mutex_unlock(pthread_mutex_t *mutex);

    Link con "-pthread" Compilar:
    gcc Linux-mutex00.cpp -pthread -o Linux-mutex
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define loop 10000

pthread_mutex_t bloqueo;
static int contador = 0;

void *pthreadHiloUno(void *arg) // Este hilo incrementa el valor del contador
{
    printf("\e[0;33mID de hilo uno:\e[0;34m%u\e[0m\n", (unsigned int)pthread_self());
    for (int i = 0; i < loop; i++)
    {
        pthread_mutex_lock(&bloqueo);
        contador++;
        pthread_mutex_unlock(&bloqueo);
    }
    pthread_exit(0);
    return 0;
}

void *pthreadHiloDos(void *arg) // Este hilo decrementa el valor de contador
{
    printf("\e[0;33mID de hilo dos:\e[0;34m%u\e[0m\n", (unsigned int)pthread_self());
    for (int i = 0; i < loop; i++)
    {
        pthread_mutex_lock(&bloqueo);
        contador--;
        pthread_mutex_unlock(&bloqueo);
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

    // Mientras el mutex este deshabilitados, el resultado nunca sera 0, que es lo que cabe esperar de la ejecución de las dos funciones.
    if (pthread_mutex_init(&bloqueo, NULL) != 0)
    {
        printf("\nInicialización de mutex fallida\n");
        return 1;
    }

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

    printf("Valor contador %d\n", contador);

    pthread_mutex_destroy(&bloqueo);
    
    return 0;
}