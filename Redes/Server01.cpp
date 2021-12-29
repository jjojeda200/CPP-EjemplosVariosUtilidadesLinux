/*
José Juan Ojeda Granados, 22-12-2021
Programando socket... Ya era hora ;-)

Big Endian:
El byte de más peso (Most Signficiante Byte) es almacenado en la dirección más
baja de memoria

Little Endian:
El byte de menos peso (Least Significant Byte) es almacenado en la dirección de
memoria más baja de memoria.

https://man7.org/linux/man-pages/man7/ip.7.html
*/

// includes necesarios
// strings / errores
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <string.h>
// simbolos standard
#include <unistd.h>
/* sockets / redes */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

/* parametros para servidor estatico */
// #define SERV_PORT       8080             /* Puerto */
// #define SERV_HOST_ADDR "192.168.30.222"  /* IP, solo soporta IPV4 */
#define BUF_SIZE 256 /* Buffer rx, tx máximo tamaño */
#define BACKLOG 5    /* Max. clientes pendientes de conexión */

int main(int argc, char *argv[])
{
    system("clear");

    if (argc > 2)
    {
        printf("%s Uso: argumento en blanco o solo una IPv4: <dotted-address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd, connfd; // Descriptores sockfd identifica el socket del servidor y connfd al del cliente.

    /*
    struct sockaddr_in {
        short            sin_family;    // Ejemplo: AF_INET (IPv4)
        unsigned short   sin_port;      // Puerto, ejemplo: 3490
        struct in_addr   sin_addr;      // struct incrustada in_addr
                                 .s_addr;
        char             sin_zero[8];   // zero this if you want to
    };
    struct in_addr {
        unsigned long s_addr;           // struct - Dirección tratada con inet_aton()
    };
    */
    //********************************************************************************
    /*
    // Internet address
    struct in_addr
    {
        uint32_t s_addr;                // address in network byte order
    };
    */

    // Creamos dos objetos basados en la clase (struct) socket, según el estándar
    struct sockaddr_in direcServidor, direcCliente;

    // Borrado del contenido del struct, eliminar basura que pudiera existir en la memoria
    memset(&direcServidor, 0, sizeof(direcServidor));

    // Asignamos valores a los datos del objeto creado en el struct - DirecServidor
    direcServidor.sin_family = AF_INET; // Ejemplo: AF_INET (IPv4)
    if (argv[1] != 0)
    {
        // Escucha en la interfaz correspondiente a la IP indicada
        direcServidor.sin_addr.s_addr = inet_addr(argv[1]);
    }
    else
    {
        // Escucha en todas las interfaces IPv4 existente
        direcServidor.sin_addr.s_addr = INADDR_ANY;
    }
    direcServidor.sin_port = htons(8080); // Puerto de escucha, ejemplo: 3490

    /* Creación del socket, SOCK_STREAM = TCP*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        fprintf(stderr, "[SERVIDOR-error]: Fallo creando el socket. (%d: %s)\n", errno, strerror(errno));
        return -1;
    }
    else
    {
        printf("[SERVIDOR]: Socket correctamente creado...\n");
    }

    /* Asignación de IP, SERV_PORT, IPV4 */
    if (bind(sockfd, (struct sockaddr *)&direcServidor, sizeof(direcServidor)) != 0)
    {
        fprintf(stderr, "[SERVIDOR-error]: Fallo el enlace del socket. (%d: %s)\n", errno, strerror(errno));
        return -1;
    }
    else
    {
        printf("[SERVIDOR]: Socket correctamente enlazado...\n");
    }

    unsigned int longitudCliente; /* Longitud de la dirección del cliente */
    int len_rx, len_tx = 0;       /* Longitud recibida y enviada, en bytes */
    // buffers de transmisión (y contenido almacenado)
    // Original: char buff_tx[BUF_SIZE] = "[SERVIDOR]: Conexión establecida";
    char buff_tx[BUF_SIZE];
    strcpy(buff_tx, "[SERVIDOR]: Conexión establecida"); // Modificación...
    char buff_rx[BUF_SIZE];                              /* buffers de recepción */

    /* A la escucha */
    if ((listen(sockfd, BACKLOG)) != 0)
    /*
    BACKLOG predefine la longitud máxima a la que puede crecer la cola de conexiones pendientes para sockfd.
    listen(sockfd, 100);
    */
    {
        fprintf(stderr, "[SERVIDOR-error]: La escucha del socket falló. %d: %s \n", errno, strerror(errno));
        return -1;
    }
    else
    {
        printf("[SERVIDOR]: Socket a la escucha, esperando...\n\n");
    }

    longitudCliente = sizeof(direcCliente);

    printf("[SERVIDOR]: Escucha en el protocolo (AF_INET):\t %d \n", direcServidor.sin_family);
    if (argv[1] != 0)
    {
        printf("[SERVIDOR]: Escucha en la dirección IPv4: \t %s \n", inet_ntoa(direcServidor.sin_addr));
    }
    else
    {
        printf("[SERVIDOR]: Escucha en la dirección (0=todas las existentes):\t %d \n", direcServidor.sin_addr.s_addr);
    }
    printf("[SERVIDOR]: Escucha en el puerto\t\t %d \n\n", ntohs(direcServidor.sin_port));

    /* Aceptar los datos de los sockets entrantes de forma iterativa */
    bool Salir = true;
    while (Salir)
    {
        connfd = accept(sockfd, (struct sockaddr *)&direcCliente, &longitudCliente);
        if (connfd < 0)
        {
            fprintf(stderr, "[SERVIDOR-error]: Conexión no aceptada. %d: %s \n", errno, strerror(errno));
            return -1;
        }
        else
        {
            printf("[SERVIDOR]: Se establecio una conexión desde el cliente: %s\n", inet_ntoa(direcCliente.sin_addr));

            // Ciclo para enviar y recibir mensajes
    // **********************************************************************
            while (1)
            {
                // Leer el mensaje del cliente, copiarlo en el buffer

                // Recibir mensaje desde el cliente
                recv(connfd, buff_rx, sizeof(buff_rx), 0);
                if (strcmp(buff_rx, "salir") == 0)
                {
                    break;
                }
                printf("[Mensaje ] <- : %s\n", buff_rx);

                // Enviar mensaje desde el servidor
                printf("[Mensaje ] -> : ");
                scanf("%*c%[^\n]", buff_tx);
                send(connfd, buff_tx, sizeof(buff_tx), 0);
                if (strcmp(buff_tx, "salir") == 0)
                {
                    break;
                }
            }
            close(connfd);

    // **********************************************************************
            system("clear");
            printf("[SERVIDOR]: Esperando conexión de nuevo cliente:\n\n");

            // Salir = false;
        }
    }

    return 0;
}

/*      // Original "Cliente se conecta, envia mensaje y desconecta"

             while (1) // Leer datos de un socket de cliente hasta que se cierre
            {
                // Leer el mensaje del cliente, copiarlo en el buffer
                len_rx = read(connfd, buff_rx, sizeof(buff_rx));
                if (len_rx == -1)
                {
                    printf("%d\n", len_rx);
                    fprintf(stderr, "[SERVIDOR-error]: connfd no se puede leer. %d: %s \n", errno, strerror(errno));
                }
                else if (len_rx == 0) // Si la longitud es 0, el socket del cliente está cerrado
                {
                    printf("%d\n", len_rx);
                    printf("[SERVIDOR]: Socket del cliente cerrado \n\n");
                    close(connfd);
                    break;
                }
                else
                {
                    write(connfd, buff_tx, strlen(buff_tx));
                    // send(connfd, " Pruebas de socket", 20, 0);
                    printf("%s \n", buff_rx);

                }
            }
*/