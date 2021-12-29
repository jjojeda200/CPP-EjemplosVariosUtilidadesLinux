/*
José Juan Ojeda Granados, 20-12-2021
Detalles manejo de Red

Big Endian:
El byte de más peso (Most Signficiante Byte) es almacenado en la dirección más
baja de memoria

Little Endian:
El byte de menos peso (Least Significant Byte) es almacenado en la dirección de
memoria más baja de memoria. 
*/
#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#define NC "\e[0m"
#define Amarillo "\e[0;33m"
#define Azul "\e[0;34m"
#define Blanco "\e[0;37m"

int main(int argc, char *argv[])
{
    system("clear");
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

    // struct in_addr direccion;
    struct sockaddr_in direccion;

    if (argc != 2)
    {
        printf("%s Uso: argumento solo una IPv4: <dotted-address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /*
    int inet_pton(int af, const char *restrict src, void *restrict dst);

    La función inet_pton() convertirá una dirección en su forma de presentación de texto estándar en su forma binaria numérica. El argumento af deberá especificar la familia de la dirección. Se admitirán las familias de direcciones AF_INET y AF_INET6. El argumento src apunta a la cadena que se está pasando. El argumento dst apunta a un búfer en el que la función almacena la dirección numérica; debe ser lo suficientemente grande para contener la dirección numérica (32 bits para AF_INET, 128 bits para AF_INET6).

    https://man7.org/linux/man-pages/man3/inet_pton.3.html
    inet_pton() devuelve 1 en caso de éxito. Se devuelve 0 si src no contiene un
    cadena de caracteres que representa una dirección de red válida en la
    familia de dirección especificada. -1 Si af no contiene una familia válida.
    inet_pton almacena la IP en direccion.sin_addr
    */
    int siError = inet_pton(AF_INET, argv[1], &direccion.sin_addr);
    if (siError == 0)
    {
        printf("No se ha proporcionado una dirección IPv4 valida %d \n", siError);
        printf("Dirección invalida\n\n");
        exit(EXIT_FAILURE);
    }
    printf("inet_pton(AR_INET, argv[1], &direccion) igual a: %d \n", siError);

    /*
    const char *inet_ntop(int af, const void *restrict src, char *restrict dst, socklen_t size);

    La función inet_ntop() convertirá una dirección numérica en una cadena de texto adecuada para la presentación. El argumento af deberá especificar la familia de la dirección. Puede ser AF_INET o AF_INET6. El argumento src apunta a un búfer que contiene una dirección IPv4 si el argumento af es AF_INET, o una dirección IPv6 si el argumento af es AF_INET6; la dirección debe estar en orden de bytes de red. El argumento dst apunta a un búfer donde la función almacena la cadena de texto resultante; no será NULO. El argumento de tamaño especifica el tamaño de este búfer, que será lo suficientemente grande para contener la cadena de texto (caracteres INET_ADDRSTRLEN para IPv4, caracteres INET6_ADDRSTRLEN para IPv6).

    https://man7.org/linux/man-pages/man3/inet_ntop.3.html
    Si tiene éxito, inet_ntop() devuelve un puntero no nulo a dst. Se devuelve NULL si hubo un error, con errno establecido para indicar el error.
    */

    // Imprime direccion.sin_addr (argv[1]) recuperada del constructor en formato Big Endian
    // Forma uno
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(direccion.sin_addr), str, INET_ADDRSTRLEN);
    printf("IP almacenada en la struct en formato Big Endian \e[0;33m(inet_ntop) %s \e[0m\n", str); 
    // Forma dos
    printf("IP almacenada en la struct en formato Big Endian \e[0;34m(inet_ntoa) %s \e[0m\n", inet_ntoa(direccion.sin_addr));

    /*
    Conjunto de funciones que traducen de el formato local (“host”) al formato
    de la red (“network”) y viceversa.
    Imprime direccion.sin_addr (argv[1]) recuperada del constructor en formato 
    Little Endian.
    Dirección perfectamente valida para realizar un envío de comprobación de
    paquetes ICMP, etc.
    */

    // Notar: la definición del tipo uint32_t, es lo mismo que usar unsigned int
    printf("\nLeast Significant Byte (Little Endian)\n");
    typedef unsigned int uint32_t;      
    uint32_t laIP1 = htonl(uint32_t(direccion.sin_addr.s_addr));
    uint16_t laIP2 = htons(uint16_t(direccion.sin_addr.s_addr));
    uint32_t laIP3 = ntohl(uint32_t(direccion.sin_addr.s_addr));
    uint16_t laIP4 = ntohs(uint16_t(direccion.sin_addr.s_addr));
    printf("htonl(uint32_t(direccion.sin_addr.s_addr)); :\e[0;33m %u\e[0m\n", laIP1);
    printf("htons(uint16_t(direccion.sin_addr.s_addr)); :\e[0;34m %u\e[0m\n", laIP2);
    // Redundante - analizar
    //printf("laIP1:\e[0;33m %u\e[0m\n", laIP3);
    //printf("laIP1:\e[0;33m %u\e[0m\n", laIP4);

    // Imprime la ip en formato: orden de bytes (Network Byte order) que es Big Endian
    // Forma uno
    unsigned int laIP;
    laIP = (direccion.sin_addr.s_addr);
    printf("\nMost Signficiante Byte (Big Endian)\n");
    printf("hex Network Byte order (Big Endian):\e[0;37m 0x%x \e[0m\n", laIP);
    printf("Network Byte order (Big Endian):\e[0;33m %u\e[0m\n", laIP);
    // Forma dos
    std::cout << "Network Byte order (Big Endian):\e[0;34m " << (direccion.sin_addr.s_addr) << "\e[0m\n";

    exit(EXIT_SUCCESS);
}