/*
    José Juan Ojeda Granados, 30-12-2021
    pthreads: manejo de hilos, crear uno, escribir en un fichero y esperar finalización
    
crea un hilo desde main
Cómo main espera a la finlaización de ese hilo
Cómo se le pasa un argumento "n" por consola a ese programa 
Cómo se le pasa un argumento "n" de entrada al hilo
El hilo abrirá un fichero y escribirá "n" líneas

    https://man7.org/linux/man-pages/man7/pthreads.7.html

    ¿?    Compile and link with -pthread.
*/
#include <iostream>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

       int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);

        int pthread_join(pthread_t thread, void **retval);

       

using std::cout; // ;-)
using std::endl;

int main(int argc, char *argv[])
{
    int inotifyDescriptor, vigila;
    char bufer[200];
    memset(&bufer, 0, sizeof(bufer));
    
    /* int inotify_init(void);
    inotify_init () inicializa una nueva instancia de inotify y devuelve un descriptor de archivo asociado con una nueva cola de eventos de inotify.
    */
    inotifyDescriptor = inotify_init();     // Descriptor de inotify

    // int inotify_add_watch(fd, const char *pathname, uint32_t mask);
    vigila = inotify_add_watch(inotifyDescriptor, "./", IN_CREATE | IN_ACCESS | IN_MODIFY | IN_OPEN | IN_DELETE );

/*
IN_CREATE   Archivo/direcctorio creado
IN_ACCESS   El fichero es accedido
IN_MODIFY   Archivo modificado
IN_OPEN     Archivo abierto
IN_DELETE)  Archivo/direcctorio borrado
*/
    cout << "\e[0;33m Descriptor de inotify:\t\t\e[0;37m" << inotifyDescriptor << "\e[0m"  << endl;
    cout << "\e[0;34m D00 del vigilante IN_CREATE:\t\e[0;37m" << vigila << "\e[0m" << endl;

/*
        struct inotify_event {
            int      wd;       // Descriptor del watch (¿Observador?)
            uint32_t mask;     // Máscara que describe el evento
            uint32_t cookie;   // Cookie única que asocia eventos relacionados (for rename(2)
            uint32_t len;      // Tamaño del campo de nombre
            char     name[];   // Nombre opcional terminado en nulo
        };
*/
    while (1)
    {
        // ssize_t read(int fd, void *buf, size_t count);
        read(inotifyDescriptor, bufer, 150);

        if (((struct inotify_event*)bufer)->mask & IN_CREATE) printf("IN_CREATE ");
        if (((struct inotify_event*)bufer)->mask & IN_ACCESS) printf("IN_ACCESS ");
        if (((struct inotify_event*)bufer)->mask & IN_MODIFY) printf("IN_MODIFY ");
        if (((struct inotify_event*)bufer)->mask & IN_OPEN) printf("IN_OPEN ");
        if (((struct inotify_event*)bufer)->mask & IN_DELETE) printf("IN_DELETE ");

        cout << "\e[0;34m wd watch:\t\e[0;37m" << ((struct inotify_event*)bufer)->wd << "\e[0m" << endl;
        cout << "\e[0;34m Máscara:\t\e[0;37m" << ((struct inotify_event*)bufer)->mask << "\e[0m" << endl;
        cout << "\e[0;34m Cookie:\t\e[0;37m" << ((struct inotify_event*)bufer)->cookie << "\e[0m" << endl;
        cout << "\e[0;34m Longitud:\t\e[0;37m" << ((struct inotify_event*)bufer)->len << "\e[0m" << endl;
        cout << "\e[0;34m Nombre:\t\e[0;37m" << ((struct inotify_event*)bufer)->name << "\e[0m" << endl;
        memset(&bufer, 0, sizeof(bufer));
    }

    /* Close inotify file descriptor. */
    close(inotifyDescriptor);

    return 0;
}