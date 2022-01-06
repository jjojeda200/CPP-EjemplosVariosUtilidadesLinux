/*
    José Juan Ojeda Granados, 06-01-2022
    exec() Reemplazar la imagen del proceso actual con una nueva imagen de proceso.

    execv(const char *pathname, char *const argv[]); 

    https://man7.org/linux/man-pages/man3/exec.3.html

    Para la ejecución de este ejercicio recomendable seguir los siguientes pasos:

    gcc AppArgumentos.cpp -o Arg
    gcc Linux-Exec.cpp -o Exec
    Exec Arg Argumento_1 Argumento_2 Argumento_3 Argumento_4

    "primer ejecutable" "segundo ejecutable" arguemento ... ... ...

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>

int main(int argc, char *argv[])
{
    system("clear");

    printf("Soy el ejecutable Execv \e[0;34m%s\e[0m, con PID \e[0;34m%i\e[0m\n",argv[0], getpid());

    if (argc < 2)
    {
        printf("%s \e[0;33mUso: Seguir comentarios del archivo fuente\n", argv[0]);
        return 0;
    }

    execv(argv[1], argv);
    
    fprintf(stderr, "Se ha producido el error %d: %s \n", errno, strerror(errno));
    
    // Solo se llega a este pinto por error al llamar a execv
    return -1;
}