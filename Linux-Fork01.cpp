/*
    José Juan Ojeda Granados, 22-12-2021
    Fork y multimples procesos hijos

    https://man7.org/linux/man-pages/man2/fork.2.html
    
    https://www.delftstack.com/es/howto/cpp/cpp-fork/
*/
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using std::cout; // Me encanta ahorrar memoria ;-)
using std::endl;

int main(int argc, char *argv[])
{
    system("clear");

    if (argc != 2)
    {
        printf("%s \e[0;33mUso: Indica el número de hijo (fork) a crear\e[0m\n", argv[0]);
        return 1;
    }

    /*
    El valor de retorno es el PID del hijo en el proceso padre y 0 es devuelto en
    el proceso hijo.
    En caso de que la llamada falle, se devuelve -1 en el proceso padre.
    Por lo tanto, podemos construir declaraciones if basadas en la evaluación del
    valor de retorno, y cada bloque if es ejecutado por el proceso correspondiente,
    resultando en una ejecución concurrente.
    */

    int argValor = atoi(argv[1]); // Convertimos el char de argv en integer
    pid_t pidRetornado;
    int status;
    int varControl = {1};
    printf("Valor de varControl antes del Fork = %d\n", varControl);
    for (int i = 0; i < argValor; i++)
    {
        pidRetornado = fork();
        varControl = i;
        if (pidRetornado > 0)
        {
            printf("\e[0;33mProceso Padre, \"getpid()\"\t\e[0;37mID del proceso: %d\e[0m\n", getpid());
            continue;
        } else if (pidRetornado == 0)
        {
            printf("\e[0;34mProceso Hijo %d \"getpid()\"\t\e[0;37mID del proceso: %d\e[0m\n", varControl, getpid());
            /*
            Sin exit los hijos se convierten en padres creando hijos en cada ciclo del bucle.
            Los hijos se quedaran en estado Zombi mientras sean esperados por el padre con un wait.            
            */
            exit(EXIT_SUCCESS);
        }
        else    // -1 si se produce error
        {
            printf("[Error] %d: %s \n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    system("ps f");
    sleep(2);
    
    for (int i = 0; i < argValor; i++)
    {
        sleep(2);
        wait(&status); // wait() o waitpid() se bloquea hasta que todos los hijos hayan terminado
        printf("\e[0;35m\t Esperando por estatus de cada hijo (Hijo %d)\n", i);
    }
    printf("\e[0;33m\n");
    system("ps f");
    return EXIT_SUCCESS;
}