/*
    José Juan Ojeda Granados, 22-12-2021
    Fork y un proceso hijo

    pid_t fork(void);
        Caracteristicas: 
	        Se utiliza para crear un nuevo proceso a partir de un proceso existente. El nuevo proceso se llama proceso hijo y el proceso original se llama proceso padre.
        Parámetros:
	        No
        valor de retorno:
	        Éxito: retorno en el proceso secundario 0, El ID del proceso hijo se devuelve en el proceso padre. pid_t es un entero sin signo.
	        Fracaso: regreso-1
	        Las dos razones principales del fracaso son:
		        1) El número actual de procesos ha alcanzado el límite superior especificado por el sistema y el valor de errno se establece en EAGAIN en este momento.
		        2) La memoria del sistema es insuficiente y el valor de errno está establecido en ENOMEM.

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
    
    int varControl = {};
    printf(" Valor de varControl antes del Fork = %d\n", varControl);
    /*
    El valor de retorno es el PID del hijo en el proceso padre y 0 es devuelto en
    el proceso hijo.
    En caso de que la llamada falle, se devuelve -1 en el proceso padre.
    Por lo tanto, podemos construir declaraciones if basadas en la evaluación del
    valor de retorno, y cada bloque if es ejecutado por el proceso correspondiente,
    resultando en una ejecución concurrente.
    */
    pid_t pidRetornado = fork();
    printf(" PID retornados por fork (!= 0 para el padre) (== 0 para el hijo): %d\n", pidRetornado);

    sleep(1); // Pausa 1 segundo, para retardar la continuación de cada sub proceso

    if (pidRetornado == -1) // -1 si se produce error
    {
        printf("[Error] %d: %s \n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pidRetornado > 0)
    {
        sleep(8); // Retardo en este bloque
        varControl = 10;
        printf("\e[0;33mProceso Padre, valor de varControl = %d\n", varControl);
        cout << "\e[0;33m Valor de retorno de fork en el proceso padre\t\e[0;37mID del proceso Hijo: " << pidRetornado << "\e[0m" << endl;
        cout << "\e[0;33m Impreso por el proceso Padre \"getpid()\"\t\e[0;37mID del proceso: " << getpid() << "\e[0m"  << endl;
        wait(nullptr);
    }
    else
    {
        sleep(2); // Retardo en este bloque
        varControl = 20;
        printf("\e[0;34mProceso Hijo, valor de varControl = %d\n", varControl);
        cout << "\e[0;34m Valor de retorno de fork en el proceso hijo\t\e[0;37mID del proceso Padre: " << pidRetornado << "\e[0m"  << endl;
        cout << "\e[0;34m Impreso por el proceso Hijo \"getpid()\"\t\t\e[0;37mID del proceso: " << getpid() << "\e[0m" << endl;
        system("ps f");
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}