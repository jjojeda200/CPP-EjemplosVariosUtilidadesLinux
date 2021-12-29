/*
    José Juan Ojeda Granados, 22-12-2021
    Fork y un proceso hijo

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

int main()
{
    system("clear");

    /*
    El valor de retorno es el PID del hijo en el proceso padre y 0 es devuelto en
    el proceso hijo.
    En caso de que la llamada falle, se devuelve -1 en el proceso padre.
    Por lo tanto, podemos construir declaraciones if basadas en la evaluación del
    valor de retorno, y cada bloque if es ejecutado por el proceso correspondiente,
    resultando en una ejecución concurrente.
    */
    pid_t pidRetornado = fork();
    printf(" PID retornados por fork (!= 0 para el padre / == 0 para el hijo): %d\n", pidRetornado);

    if (pidRetornado == -1)
    {
        printf("[Error] %d: %s \n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pidRetornado > 0)
    {
        cout << "\e[0;33m Impreso por el proceso Padre\t \e[0;37mID del proceso: " << getpid() << "\e[0m"  << endl;
        wait(nullptr);
    }
    else
    {
        cout << "\e[0;34m Impreso por el proceso Hijo\t \e[0;37mID del proceso: " << getpid() << "\e[0m" << endl;
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}