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

      Este código lee el objeto "/miObjetoMemoria"
      El objecto se encontrara en: /dev/shm

      https://man7.org/linux/man-pages/man7/shm_overview.7.html

   Link con "-lrt" Compilar:
   gcc Linux-ShareMemory02R.cpp -lrt -o SM02R
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
   void *ptr;
   struct stat shmobj_st;
/*
   struct stat {
      dev_t st_dev;     // ID del dispositivo que contiene el archivo * /
      ino_t st_ino;     // Número de inodo * /
      mode_t st_mode;   // Tipo y modo de archivo * /
      nlink_t st_nlink; // Número de enlaces físicos * /
      uid_t st_uid;     // ID de usuario del propietario * /
      gid_t st_gid;     // ID de grupo del propietario * /
      dev_t st_rdev;    // ID del dispositivo (si es un archivo especial) * /
      off_t st_size;    // Tamaño total, en bytes * /
      blksize_t st_blksize;   // Tamaño de bloque para E / S del sistema de archivos * /
      blkcnt_t st_blocks;     // Número de bloques 512B asignados * /

      struct timespec st_atim; // Hora del último acceso
      struct timespec st_mtim; // Hora de la última modificación
      struct timespec st_ctim; // Hora del último cambio de estado
*/

   // Abre en modo lectura el objeto Share Memory
   // int shm_open(const char *name, int oflag, mode_t mode);
   descriptorArchivo = shm_open(NOMBRE_SMOBJ, O_RDONLY, 00400);
   if (descriptorArchivo == -1)
   {
      printf("Error en el descriptor de archivo %s\n", strerror(errno));
      exit(1);
   }
   // Adquiere el estatus de la memoria compartida
   // int fstat(int fd, struct stat *statbuf);
   if (fstat(descriptorArchivo, &shmobj_st) == -1)
   {
      printf("Error fstat: %s \n", strerror(errno));
      exit(1);
   }
   // Asigna el objeto de memoria compartida en el espacio de direcciones virtuales del proceso de llamada.
   // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
   ptr = mmap(NULL, shmobj_st.st_size, PROT_READ, MAP_SHARED, descriptorArchivo, 0);
   if (ptr == MAP_FAILED)
   {
      printf("mmap falló en el proceso de lectura: %s\n", strerror(errno));
      exit(1);
   }

   printf("%s \n", ptr);
   close(descriptorArchivo);

   return 0;
}