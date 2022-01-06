/*
    José Juan Ojeda Granados, 05-01-2022
    Programar Colas de Mensajes con prioridad (POSIX Message Queues) 

    Comando Kill, ps

    https://man7.org/linux/man-pages/man7/mq_overview.7.html

    https://man7.org/linux/man-pages/man3/mq_open.3.html
    mqd_t mq_open(const char *name, int oflag);
    mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

                struct mq_attr 
                {
                long mq_flags;      / * Banderas (ignoradas para mq_open ()) * /
                long mq_maxmsg;     / * Máx. # de mensajes en cola * /
                long mq_msgsize;    / * Máx. tamaño del mensaje (bytes) * /
                long mq_curmsgs;    / * # de mensajes actualmente en cola
                                       (ignorado para mq_open ()) * /
                };

    https://man7.org/linux/man-pages/man3/mq_send.3.html
    int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

    https://man7.org/linux/man-pages/man3/mq_receive.3.html
    ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);

    Link con "-lrt" Compilar:
    gcc Linux-Mensajes00.cpp -lrt -o Linux-Mensajes00
*/
#include <mqueue.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>

int main(int argc, char *argv[])
{
    system("clear");

    if (argc != 2)
    {
        printf("%s \e[0;33mUso: Indicar nombre de la cola (\\nombre)\n", argv[0]);
        return 1;
    }

    mqd_t mqDescriptor;
    char mensajebufer[8192]; // Tamaño especificado en el sistemas
    unsigned int prioridad, numeroCaracteres;

    // Importante: por defecto las colas de mensajes (mq) se crean en /dev/mqueue/
    // mqd_t mq_open(const char *name, int oflag);
    mqDescriptor = mq_open(argv[1], O_RDONLY);
    if (mqDescriptor == -1)
    {
        fprintf(stderr, "Se ha producido un error %d: %s \n", errno, strerror(errno));
        return -1;
    }

    numeroCaracteres = mq_receive(mqDescriptor, mensajebufer, 8192, &prioridad);

    printf("\e[0;33mMensaje recibido: %s, con prioridad %d\e[0m \n", mensajebufer, prioridad);
    printf("\e[0;33mNúmero de caracteres recibido: %u\e[0m \n", numeroCaracteres);
    
    mq_close(mqDescriptor);
    
    return 0;
}