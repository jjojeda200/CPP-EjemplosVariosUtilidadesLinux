/*
José Juan Ojeda Granados, 23-12-2021
Detalles manejo de formatos en Red

Big Endian:
El byte de más peso (Most Signficiante Byte) es almacenado en la dirección más
baja de memoria

Little Endian:
El byte de menos peso (Least Significant Byte) es almacenado en la dirección de
memoria más baja de memoria.

Link con "-lresolv" Compilar:
gcc RedesVarios01.cpp -lresolv -o RedesVarios01
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define errExit(msg)        \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    char buf[100];
    struct in_addr addr;
    int bits;

    if (argc < 2)
    {
        fprintf(stderr,
                "Uso: %s [addr-init-value]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Si se proporciona argv[2] (un valor numérico), lo utilíza para inicializar
    el búfer de salida dado a inet_net_pton(), de modo que podamos ver que
    inet_net_pton() inicializa solo los bytes necesarios para el número de red.
    Si no se proporciona argv [2], inicialice el búfer a cero (como es la práctica
    recomendada). */

    addr.s_addr = (argc > 2) ? strtod(argv[2], NULL) : 0;

    /* Convierte el número de la red de presentación en argv[1] a binario. */

    bits = inet_net_pton(AF_INET, argv[1], &addr, sizeof(addr));
    if (bits == -1)
        errExit("inet_net_ntop");

    printf("inet_net_pton() retorna: %d\n", bits);

    /* Convierte el formato binario de nuevo a presentación, usando 'bits'
    devueltos por inet_net_pton(). */

    if (inet_net_ntop(AF_INET, &addr, bits, buf, sizeof(buf)) == NULL)
        errExit("inet_net_ntop");

    printf("inet_net_ntop() cedida:  %s\n", buf);

    /* Muestra 'addr' en forma sin procesar (en orden de bytes de red), para que
    podamos ver los bytes que no se muestran en inet_net_ntop(); algunos de esos
    bytes pueden no haber sido tocados por inet_net_ntop(), por lo que todavía
    tendrán cualquier valor inicial que se especificó en argv[2]. */

    printf("Dirección sin procesar:              %x\n", htonl(addr.s_addr));

    exit(EXIT_SUCCESS);
}
