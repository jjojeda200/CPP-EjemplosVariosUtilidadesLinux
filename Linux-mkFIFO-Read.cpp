/*
    José Juan Ojeda Granados, 01-02-2022
    Programar FIFOs también conocidos "Named Pipes", son un mecanismo de comunicación entre procesos.
    mkfifo - construye un fichero especial FIFO (una tubería con nombre)

    Este ejecutable es el encargado de leer

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


int main(int argc, char *argv[])
{
    system("clear");

    int descriptorArchivo, numeroBytes;
    char bufer[250];
    printf("Se busca el archivo especial ./miFIFO, para leer el mansaje\n");
    
/*
       int mkfifo(const char *pathname, mode_t mode);
       int mkfifoat(int dirfd, const char *pathname, mode_t mode);
*/
    mkfifo("./miFIFO", 0666);

    descriptorArchivo = open("./miFIFO", O_RDONLY);
    numeroBytes = read(descriptorArchivo, bufer, sizeof(bufer));
    printf("Número de bytes recibidos: %d \n", numeroBytes);
    printf("RX: %s \n", bufer);
    close(descriptorArchivo);

    return 0;
}