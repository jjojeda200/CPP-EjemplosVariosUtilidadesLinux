/*
    Original de Amaia - WhileTrueThenDream
    https://github.com/WhileTrueThenDream

    José Juan Ojeda Granados, 06-01-2022
    Programar zonas de Memoria compartidas (POSIX Shared Memory)

    Se utilizan tres programas:
      El primero creará un objeto de memoria compartida
      El segundo escribirá un texto en el objeto de memoria compartida
      El tercero leerá de ese objeto de memoria compartida

      Además se mapeará el objeto de memoria en la memoria virtual del programa. Las llamadas al sistema realizadas serán:
         int shm_open(...);
         void *mmap(.., MAP_SHARED ... );
         int ftruncate(...);
         int fstat(...);

      Este código crea el objeto "/miObjetoMemoria"
      El objecto se encontrara en: /dev/shm

      https://man7.org/linux/man-pages/man7/shm_overview.7.html

   Link con "-lrt" Compilar:
   gcc Linux-ShareMemory00C.cpp -lrt -o SM00C
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define SIZEOF_SMOBJ 200
#define NOMBRE_SMOBJ "/miObjetoMemoria"

int main(void)
{
   system("clear");

   int descriptorArchivo;

   // Crear el objeto Share Memory
   // int shm_open(const char *name, int oflag, mode_t mode);
   descriptorArchivo = shm_open(NOMBRE_SMOBJ, O_CREAT | O_RDWR, 00700);
   if (descriptorArchivo == -1)
   {
      printf("Error en el descriptor de archivo %s\n", strerror(errno));
      exit(1);
   }
   // truncar un archivo a una longitud especificada
   // int ftruncate(int fd, off_t length);
   if (-1 == ftruncate(descriptorArchivo, SIZEOF_SMOBJ))
   {
      printf("Error: no se puede cambiar el tamaño de la memoria compartida\n");
      exit(1);
   }
   system("ls -la /dev/shm/");
   printf("\n\e[0;34mObjeto Share Memory creadom\e[0m\n");
   
   close(descriptorArchivo);

   return 0;
}