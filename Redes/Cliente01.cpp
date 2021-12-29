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

#include <stdlib.h>

#define SERVER_ADDRESS "192.168.30.222" /* IP del servidor*/
#define PORT 8080

/* Secuencia de test */
char buff_tx[256]; // Buffer de transmisión
char buff_rx[256]; // Buffer de recepción

/* Este cliente se conecta, envía un texto (y se desconecta originalmente) */
int main()
{
    system("clear");

    int sockfd;
    struct sockaddr_in direcCliente;

    /* Creación del socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("[CLIENTE ]: Fallo creando el socket.\n");
        return -1;
    }
    else
    {
        printf("[CLIENTE ]: Socket correctamente creado...\n");
    }

    memset(&direcCliente, 0, sizeof(direcCliente));

    /* Asigna IP, PORT */
    direcCliente.sin_family = AF_INET;
    direcCliente.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    direcCliente.sin_port = htons(PORT);

    /* Intenta conectar el socket del cliente al socket del servidor */
    if (connect(sockfd, (struct sockaddr *)&direcCliente, sizeof(direcCliente)) != 0)
    {
        printf("[CLIENTE ]: Fallo de conexión con el servidor...\n");
        return -1;
    }

    printf("[CLIENTE ]: Conectado con el servidor...\n");

    // **********************************************************************
    while (1)
    {
        // Enviar mensaje desde el cliente
        printf("[Mensaje ] -> : ");
        scanf("%*c%[^\n]", buff_tx);
        send(sockfd, buff_tx, sizeof(buff_tx), 0);
        if (strcmp(buff_tx, "salir") == 0)
        {
            break;
        }

        // Recibir mensaje desde el servidor
        recv(sockfd, buff_rx, sizeof(buff_rx), 0);
        if (strcmp(buff_rx, "salir") == 0)
        {
            break;
        }
        printf("[Mensaje ] <- : %s\n", buff_rx);
        printf("Tamaño <- %lu \n", strlen(buff_rx));
    }
    // **********************************************************************

    /* close the socket */
    printf("[CLIENTE ]: Cerrando conexión con el servidor...\n\n");
    close(sockfd);

    return 0;
}

/*
    // Original "Cliente se conecta, envia mensaje y desconecta"

    // Envio secuencia de test
    while (1)
    {
    write(sockfd, buff_tx, strlen(buff_tx));
    read(sockfd, buff_rx, sizeof(buff_rx));
    printf("%s \n", buff_rx);
    }



        printf("Tamaño <- %lu \n", strlen(buff_rx));
        printf("Tamaño <- %lu \n", sizeof(buff_rx));
 */