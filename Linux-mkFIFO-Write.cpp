/*
    José Juan Ojeda Granados, 01-02-2022
    Programar FIFOs también conocidos "Named Pipes", son un mecanismo de comunicación entre procesos.
    mkfifo - construye un fichero especial FIFO (una tubería con nombre)

    Este ejecutable es el encargado de escribir

    https://man7.org/linux/man-pages/man7/fifo.7.html

    https://man7.org/linux/man-pages/man3/mkfifo.3.html
*/

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*
       int mkfifo(const char *pathname, mode_t mode);
       int mkfifoat(int dirfd, const char *pathname, mode_t mode);
*/

int main(int argc, char *argv[])
{
    system("clear");

    int descriptorArchivo;
    char bufer[] = "Prueba 123456 ...";

    mkfifo("./miFIFO", 0666);

    descriptorArchivo = open("./miFIFO", O_WRONLY);
    write(descriptorArchivo, bufer, sizeof(bufer));
    printf("TX: %s \n", bufer);
    close(descriptorArchivo);

    return 0;
}