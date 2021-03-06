/*
    José Juan Ojeda Granados, 07-01-2022
    Mapear o des asignar archivos o dispositivos en la memoria

    https://man7.org/linux/man-pages/man2/mmap.2.html

    Este programa es el ejemplo de la man-pages del enlace anterior:

    Argumentos de entrada
    - argv [1] -> nombre de archivo y ruta
    - argv [2] -> Número de caracteres a desplazarse
    - argv [3] -> Número de caracteres que mostrara desde el desplazamiento en la salida estándar

    Ejemplo:
    Linux-mmap (un carácter A / * 'a' se reemplazará en el archivo con el carácter 'A'

    He debido usar static_cast en la llamada a mmap para evitar error de convención de tipos en compilación (eliminar static_cast <char*> para reproducir)
*/
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    char *addr;
    int fd;
    struct stat sb;
    off_t offset, pa_offset;
    size_t length;
    ssize_t s;

    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "%s file offset [length]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");

    if (fstat(fd, &sb) == -1) /* To obtain file size */
        handle_error("fstat");

    offset = atoi(argv[2]);
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
    /* offset for mmap() must be page aligned */

    if (offset >= sb.st_size)
    {
        fprintf(stderr, "offset is past end of file\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 4)
    {
        length = atoi(argv[3]);
        if (offset + length > sb.st_size)
            length = sb.st_size - offset;
        /* Can't display bytes past end of file */
    }
    else
    { /* No length arg ==> display to end of file */
        length = sb.st_size - offset;
    }

    addr = static_cast<char*> (mmap(NULL, length + offset - pa_offset, PROT_READ,
                MAP_PRIVATE, fd, pa_offset));
    if (addr == MAP_FAILED)
        handle_error("mmap");

    s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
    if (s != length)
    {
        if (s == -1)
            handle_error("write");

        fprintf(stderr, "partial write");
        exit(EXIT_FAILURE);
    }

    munmap(addr, length + offset - pa_offset);
    close(fd);

    exit(EXIT_SUCCESS);
}