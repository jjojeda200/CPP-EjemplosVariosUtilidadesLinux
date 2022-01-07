/*
    Original de Amaia - WhileTrueThenDream
    https://github.com/WhileTrueThenDream

    José Juan Ojeda Granados, 07-01-2022
    Mapear o des asignar archivos o dispositivos en la memoria

    https://man7.org/linux/man-pages/man2/mmap.2.html


    Este programa:
    - Carga un archivo en la memoria virtual con mmap
    - Busca ocurrencias de un carácter determinado en el archivo
    - Reemplaza todas las ocurrencias por otro carácter indicado en el archivo.

    Argumentos de entrada
    - argv [1] -> nombre de archivo y ruta
    - argv [2] -> carácter a buscar
    - argv [3] -> carácter que reemplaza a los encontrados

    Ejemplo:
    Linux-mmap ./nombreArchivo 'a' se reemplazará en el archivo con el carácter 'A'

    https://linuxhint.com/using_mmap_function_linux/

    He debido usar static_cast en la llamada a mmap para evitar error de convención de tipos en compilación (eliminar static_cast <char*> para reproducir)
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char *addr, *file_name;
    int fd;
    struct stat file_st;
    char seekchar, newchar;

    if (argc == 4)
    {
        file_name = argv[1];
        seekchar = *argv[2];
        newchar = *argv[3];
    }
    else
    {
        printf("Error en el numero de argumentos \n");
        exit(1);
    }

    if (-1 == (fd = open(file_name, O_RDWR))) // Abre el archivo en modo lectura/escritura
    {
        perror("Error abriendo el archivo \n");
        exit(1);
    }

    fstat(fd, &file_st); // Cargando estatus del archivo
    // Asigna el objeto de memoria compartida en el espacio de direcciones virtuales del proceso de llamada.
    // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
    addr = static_cast<char*>(mmap(NULL, file_st.st_size, PROT_WRITE, MAP_SHARED, fd, 0));
    if (addr == MAP_FAILED)
    {
        printf("mmap falló en el proceso: %s\n", strerror(errno));
        exit(1);
    }

    printf("\nContenido del fichero antes:\n%s \n", addr);

    for (size_t i = 0; i < file_st.st_size; i++)
    {
        if (addr[i] == seekchar)
            (addr[i] = newchar);
    }

    printf("\nContenido del fichero después:\n%s \n", addr);

    return 0;
}