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

      Este código escribe el objeto "/miObjetoMemoria"
      El objecto se encontrara en: /dev/shm

      https://man7.org/linux/man-pages/man7/shm_overview.7.html

   Link con "-lrt" Compilar:
   gcc Linux-ShareMemory01W.cpp -lrt -o SM01W
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

#define NOMBRE_SMOBJ "/miObjetoMemoria"

int main(void)
{
   int descriptorArchivo;
   char bufer[] = "Ejecutando el proceso de escritura en la memoria compartida";
   void *ptr;

   // Abre en modo escritura el objeto Share Memory
   // int shm_open(const char *name, int oflag, mode_t mode);
   descriptorArchivo = shm_open(NOMBRE_SMOBJ, O_RDWR, 00200);
   if (descriptorArchivo == -1)
   {
      printf("Error en el descriptor de archivo %s\n", strerror(errno));
      exit(1);
   }
   // Asigna el objeto de memoria compartida en el espacio de direcciones virtuales del proceso de llamada.
   // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
   ptr = mmap(NULL, sizeof(bufer), PROT_WRITE, MAP_SHARED, descriptorArchivo, 0);
   if (ptr == MAP_FAILED)
   {
      printf("mmap falló en el proceso de escritura: %s\n", strerror(errno));
      exit(1);
   }

   memcpy(ptr, bufer, sizeof(bufer));
   printf("%d \n", (int)sizeof(bufer));
   close(descriptorArchivo);

   return 0;
}