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
    gcc Linux-mutex01.cpp -pthread -o Linux-mutex
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t tid[2];
int contador;
pthread_mutex_t bloqueo;

void *trythis(void *arg)
{
    pthread_mutex_lock(&bloqueo);

    unsigned long i = 0;
    contador += 1;
    printf("\n Trabajo %d en ejecución\n", contador);

    for (i = 0; i < (0xFFFFFFFF); i++);

    printf("\n Trabajo %d has finalizado\n", contador);

    pthread_mutex_unlock(&bloqueo);

    return NULL;
}

int main(void)
{
    system("clear");

    int i = 0;
    int error;

    if (pthread_mutex_init(&bloqueo, NULL) != 0)
    {
        printf("\nInicialización de mutex fallida\n");
        return 1;
    }

    while (i < 2)
    {
        error = pthread_create(&(tid[i]), NULL, &trythis, NULL);
        if (error != 0)
            printf("\nHilo (Thread) no se puede crear :[%s]",
                   strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&bloqueo);

    return 0;
}