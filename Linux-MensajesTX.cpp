/*
    José Juan Ojeda Granados, 04-01-2022
    Programar Colas de Mensajes (POSIX Message Queues) 

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

    mqd_t mqDescriptor;
    char mensajebufer[100];

    // Importante: por defecto las colas de mensajes (mq) se crean en /dev/mqueue/
    // mqd_t mq_open(const char *name, int oflag);
    mqDescriptor = mq_open("/miColaMen", O_RDWR | O_CREAT, 0664, NULL);
    if (mqDescriptor == -1)
    {
        fprintf(stderr, "Se ha producido un error %d: %s \n", errno, strerror(errno));
        return -1;
    }

    printf("\e[0;33mIntroduce un mensaje (máximo 100 caracteres):\e[0m ");
    scanf("%s", mensajebufer);

    mq_send(mqDescriptor, mensajebufer, strlen(mensajebufer) + 1, 0);

    mq_close(mqDescriptor);
    
    return 0;
}