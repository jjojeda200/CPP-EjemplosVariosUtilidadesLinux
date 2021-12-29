/*
    Original de Amaia - WhileTrueThenDream
    https://github.com/WhileTrueThenDream
    https://www.youtube.com/channel/UC2vyzCWiSVZr_1JcI8JtrFw

    José Juan Ojeda Granados, 23-12-2021
    Programar "ls -l | wc > salida.txt"
    El archivo salida.txt debe existir
    TODO: manejo de errores
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

#define READ_END 0  /* index pipe extremo lectura */
#define WRITE_END 1 /* index pipe extremo escritura */

#define FILE_NAME "salida.txt" /* nombre del archivo donde escribir */

int main(int argc, char *argv[])
{
    system("clear");
    int fd1[2], fd2;
    int status, pid;

    pipe(fd1); /* Comunica ls y wc */
    printf("FD creados por pipe(fd1) y almacenados en: fd1[0]:\e[0;37m %d\e[0m y fd1[1]:\e[0;37m %d\e[0m\n", fd1[0], fd1[1]);
    printf("FD creados por pipe(fd1) y almacenados en: fd1[READ_END]:\e[0;37m %d\e[0m y fd1[WRITE_END]:\e[0;37m %d\e[0m\n", fd1[READ_END], fd1[WRITE_END]);

    pid = fork();
    if (pid == -1)
    {
        printf("[Error] %d: %s \n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pid == 0) // Hijo UNO ejecuta "ls -l"
    /*
    Condición que identifica al proceso del hijo generado por el primer fork, se encargara de ejecutar el programa del sistema "ls -la" y enviar su salida a
    través de escritura por el pipe, cuya lectura realizara el segundo proceso hijo
    que se crea en otro hilo por el padre.
    */
    {
        printf("\e[0;34m Impreso por el primer proceso hijo\t \e[0;37mID del proceso: %d \e[0m\n", getpid());
        close(fd1[READ_END]); // Cerrar extremo lectura no necesario, no se va a leer
        printf("Cerrado: fd1[READ_END]\e[0;37mID %d\e[0m del proceso ID:\e[0;37mID %d\e[0m\n", fd1[READ_END], getpid());

        dup2(fd1[WRITE_END], (STDOUT_FILENO)); // stdout se redirige a fd1[WRITE_END]
        close(fd1[WRITE_END]);

        execlp("/bin/ls", "ls", "-la", NULL);
    }
    else // Padre
    {
        printf("\e[0;33m Impreso por el proceso Padre\t\t \e[0;37mID del proceso: %d \e[0m\n", getpid());
        close(fd1[WRITE_END]); // Cerrar extremo escritura no necesario y el hijo no lo hereda

        pid = fork();
        // El hijo segundario solo hereda el pipe de lectura abierto 
        if (pid == 0) // Hijo DOS ejecuta "wc"
        {
            printf("\e[0;31m Impreso por el segundo proceso hijo\t \e[0;37mID del proceso: %d \e[0m\n", getpid());
            fd2 = open(FILE_NAME, O_WRONLY);
            dup2(fd1[READ_END], STDIN_FILENO);
            close(fd1[READ_END]);
            printf("Cerrado: fd1[READ_END]\e[0;37mID %d\e[0m del proceso ID:\e[0;37mID %d\e[0m\n", fd1[READ_END], getpid());
            
            dup2(fd2, STDOUT_FILENO);

            execlp("/usr/bin/wc", "wc", NULL);
        }
        else // Padre
        {
            close(fd1[READ_END]); // Cerrar extremo lectura no necesario
        }
    }

    // wait para cada hijo
    wait(&status);
    wait(&status);

    return 0;
}
/*

PIPE
pipe() crea una tubería, un canal de datos unidireccional que se puede utilizado para la comunicación entre procesos. La matriz pipefd se utiliza para devuelve dos descriptores de archivo que se refieren a los extremos de la tubería. pipefd[0] se refiere al extremo leído de la tubería. pipefd[1] se refiere al extremo de escritura de la tubería. Datos escritos al final de escritura de la tubería es almacenada en búfer por el kernel hasta que se lee de la lectura extremo de la tubería.

DUP
La llamada al sistema dup () asigna un nuevo descriptor de archivo que se refiere a la misma descripción de archivo abierto que el descriptor oldfd . (Para una explicación de las descripciones de archivos abiertos, consulte open(2)).
Se garantiza que el número de descriptor de archivo es el número más bajo descriptor de archivo que no se utilizó en el proceso de llamada.

Después de una devolución exitosa, los descriptores de archivo nuevos y antiguos pueden utilizarse indistintamente. Dado que los dos descriptores de archivo se refieren a la misma descripción de archivo abierto, comparten el desplazamiento de archivo y el archivo banderas de estado; por ejemplo, si el desplazamiento del archivo es modificado por usando lseek (2) en uno de los descriptores de archivo, el desplazamiento también es cambiado para el otro descriptor de archivo.

Los dos descriptores de archivo no comparten indicadores de descriptor de archivo (el bandera close-on-exec). El indicador close-on-exec ( FD_CLOEXEC ; consulte fcntl (2) ) para el descriptor duplicado está desactivado.

DUP2
La llamada al sistema dup2 () realiza la misma tarea que dup (), pero en lugar de utilizar el descriptor de archivo no utilizado con el número más bajo, utiliza el número de descriptor de archivo especificado en newfd. En otras palabras, el descriptor de archivo newfd se ajusta para que ahora se refiera a la misma descripción de archivo abierto que oldfd.
Si el descriptor de archivo newfd estaba abierto anteriormente, se cierra antes de ser reutilizado; el cierre se realiza en silencio (es decir, los errores durante el cierre no son informados por dup2 ()).
Los pasos para cerrar y reutilizar el descriptor de archivo newfd se realizan de forma atómica. Esto es importante, porque intentar implementar una funcionalidad equivalente usando close (2) y dup () estaría sujeto a condiciones de carrera, por lo que newfd podría reutilizarse entre los dos pasos. Tal reutilización podría ocurrir porque el programa principal es interrumpido por un manejador de señales que asigna un descriptor de archivo, o porque un subproceso paralelo asigna un descriptor de archivo.
*/