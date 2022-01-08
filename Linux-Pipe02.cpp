/*
    José Juan Ojeda Granados, 08-01-2022

    pipe() crea una tubería, un canal de datos "unidireccional" que se puede utilizado para la comunicación entre procesos. La array pipefd se utiliza para retornar dos descriptores de archivo que se refieren a los extremos de la tubería.
        pipefd [0] se refiere al extremo de lectura de la tubería.
        pipefd [1] se refiere al extremo de escritura de la tubería.

    https://man7.org/linux/man-pages/man2/pipe.2.html

    Compilar con "-lstdc++" en gcc:
    gcc Linux-Pipe02.cpp -o Linux-Pipe02 -lstdc++
    g++ Linux-Pipe02.cpp -o Linux-Pipe02
*/
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    system("clear");

    int fd0[2]; // Primer pipe - Para dirección hijo --> padre
    int fd1[2]; // Segundo pipe - Para dirección padre --> hijo
    pid_t pRetornado;
    char string0[] = "\e[0;34m Mensaje enviado por el proceso hijo\e[0m\n";
    char string1[] = "\e[0;33m Mensaje enviado por el proceso padre\e[0m\n";
    char readbuffer0[80];
    char readbuffer1[80];

    int resultado0 = pipe(fd0);
    int resultado1 = pipe(fd1);
    if (resultado0 < 0 && resultado1 < 0)
    {
        printf("Error creando Pipes\n");
        return 1;
    }
    pRetornado = fork();
    printf("ID's retornados por fork (!= 0 para el padre) (== 0 para el hijo): %d\n", pRetornado);
    if (pRetornado == -1)
    {
        printf("[Error] %d: %s \n", errno, strerror(errno));
        return 1;
    }
    if (pRetornado == 0) // Proceso Hijo
    {
        close(fd0[0]); // Cerrando pipe extremo lectura padre - no se usa
        close(fd1[1]); // Cerrando pipe extremo escritura padre - no se usa

        printf("\e[0;34mProceso Hijo, \"getpid()\"\t\e[0;37mPID: %d\e[0;34m\tPPID:\e[0;37m %d\e[0m\n", getpid(), getppid());
        printf("\e[0;34mProceso hijo lee en el fd1[0] del pipe:\e[0;37m\t\t%d\e[0m\n", fd1[0]);
        printf("\e[0;34mProceso hijo escribe en el fd0[1] del pipe:\e[0;37m\t%d\e[0m\n\n", fd0[1]);

        // Envia cadena a través del lado de salida de la tubería
        printf("\e[0;34mHijo escribiendo en su extremo del pipe\e[0m\n");
        write(fd0[1], string0, sizeof(string0));

        printf("\e[0;34mHijo (%d) leyendo de su extremo del pipe\e[0m\n", getpid());
        read(fd1[0], readbuffer1, sizeof(readbuffer1)); // pipe extremo lectura hijo 
        printf("\e[0;34mRecibiendo mensaje del padre:\e[0m\t%s", readbuffer1);

        close(fd0[1]); // Cerrando pipe extremo escritura hijo
        close(fd1[0]); // Cerrando pipe extremo lectura hijo
        
        return 0;
    }
    else // Proceso Padre
    {
        close(fd0[1]); // Cerrando pipe extremo escritura hijo - no se usa
        close(fd1[0]); // Cerrando pipe extremo lectura hijo - no se usa

        printf("\e[0;33mProceso Padre, \"getpid()\"\t\e[0;37mPID: %d\e[0;33m\tPPID:\e[0;37m %d\e[0m\n", getpid(), getppid());
        printf("\e[0;33mProceso padre lee en el fd0[0] del pipe:\e[0;37m\t%d\e[0m\n", fd0[0]);
        printf("\e[0;33mProceso padre escribe en el fd1[1] del pipe:\e[0;37m\t%d\e[0m\n\n", fd1[1]);
        close(fd0[1]);  // Cerrando pipe extremo escritura padre

        sleep(1);

        printf("\e[0;33mPadre (%d) leyendo de su extremo del pipe\e[0m\n", getpid());
        read(fd0[0], readbuffer0, sizeof(readbuffer0)); // pipe extremo lectura padre
        printf("\e[0;33mRecibiendo mensaje del hijo:\e[0m\t%s", readbuffer0);
        
        // Envia cadena a través del lado de salida de la tubería
        printf("\e[0;33mPadre escribiendo en su extremo del pipe\e[0m\n");
        write(fd1[1], string1, sizeof(string1));

        close(fd0[0]); // Cerrando pipe extremo lectura padre
        close(fd1[1]); // Cerrando pipe extremo escritura padre
        
    }
    return 0;
}