Sockets TCP

https://www.whiletruethendream.com/sockets-tcp/

https://programmerclick.com/article/49161264435/

Qué es un socket

Un socket es un mecanismo de comunicación entre procesos (IPC del Inglés Inter Process Communication), como lo son los «messages queues» o los «pipes».
Dominios de comunicación.

Apertura de un socket, mediante la función socket(). La función socket() no hace absolutamente nada, salvo devolvernos y preparar un descriptor de fichero que el sistema posteriormente asociará a una conexión en red.

Según dónde se estén ejecutando las aplicaciones a comunicar mediante sockets se definen dos dominios:

1.- Dominio Unix

Si las aplicaciones están dentro de la misma máquina (mismo host) los sockets los definiremos dentro del dominio UNIX (AF_UNIX, AF_LOCAL).

2.- Dominio de internet

Si las aplicaciones corren en hosts conectados a través de la red necesitaremos sockets en el dominio de internet: AF_INET (IPv4) o AF_INET6 (IPv6)
Tipos de sockets

En función de cómo se realice la entrega de los datos se definen los siguientes tipos:
«Stream Sockets» (SOCK_STREAM)

    La recepción de los datos está garantizada.
    No hay concepto de mensajes, es un bytestream (flujo de datos continuo) como si se tratara de un pipe.
    Transmisión bidireccional
    Se describen como «orientados a conexión»
    En el dominio de Internet a los «socket streams» emplean el protocolo de comunicación TCP . A los socket streams de dominio Internet se les denomina TCP sockets.

«Datagram sockets» (SOCK_DGRAM)

    Los mensajes son llamados datagramas, es decir, este tipo de sockets sí que implementan el concepto de «mensaje»
    La transmisión de datos no está garantizada, es decir, pueden perderse mensajes, llegar duplicados o desordenados. (NOTA: en el dominio AF_UNIX sí que son seguros los datagramas, el Kernel se encarga de que lleguen todos los datos)
    En el dominio de internet los «datagram sockets» se dominan «UDP sockets»

«Raw Sockets»

Permite definier un protocolo de transporte propio.
Sockets TCP

Volviendo al título original :

Los «Sockets TCP» son un mecanismo de comunicación entre aplicaciones que corren en máquinas diferentes conectadas por Internet. La entrega de datos entre ambas aplicaciones está garantizada.
Sockets pasivos y activos

Un socket pasivo es un socket que «escucha» y se mantiene a la espera de que un socket activo trate de conectarse a él. Éste es el tipo de socket que implementa un programa «servidor» esperando conexiones entrantes de clientes.

Un socket activo es un socket que trata activamente de conectarse a un socket pasivo. Éste es el tipo de socket que implementa un cliente que trata de conectarse a un servidor.
Llamadas al sistema utilizadas en los TCP Sockets
Dirección del Socket

La dirección del socket viene dada por una dirección IP que identifica a la máquina (host) dentro de la red y por un número de puerto que identifica a la aplicación que crea el socket. Para IPv4 se define la siguiente estructura:

struct sockaddr_in 
{
   sa_family_t    sin_family; /* address family: AF_INET */
   in_port_t      sin_port;   /* port in network byte order */
   struct in_addr sin_addr;   /* internet address */
 };

/* Internet address. */
struct in_addr 
{
    uint32_t  s_addr;   /* address in network byte order */ 
};

Por ejemplo:

A continuación se definieran la dirección IP y el puerto del servidor. Los valores se programarán de manera «hardcoded».

    La dirección del servidor es 192.168.0.21
    El puerto usado por el servidor es 8080

struct sockaddr_in servaddr; 

servaddr.sin_family      = AF_INET; 
servaddr.sin_addr.s_addr = inet_addr("192.168.0.20"); 
servaddr.sin_port        = htons(8080); 

La función inet_addr convierte el string constante «192.168.0.21» en una dirección válida para la red es decir:

    La dirección es de tipo uint32_t
    El orden de bytes (Network Byte order) es Big Endian que es el formato adecuado para la red.

La función htons ordena los bytes del número de puerto conviertiéndolos al formato de red (Big Endian).

NOTA: La arquitectura de una computadora determina el formato el orden en el qeu se almacenan los bytes en memoria. Cuando los bytes viajan por la red es preciso que lo hagan siempre en el mismo formato de orden, independientemente de la computadora que los envíe o recibe. Es por éso que es preciso adaptarlos.

Big Endian:

El byte de más peso (Most Signficiante Byte) es almacenado en la dirección más baja de memoria

Little Endian:

El byte de menos peso (Least Significant Byte) es almacenado en la dirección de memoria más baja de memoria.

Diseño del servidor

Hay dos diseños comunes a la hora de programar un servidor: Iterativo y concurrente. En el primer caso los clientes son atendidos de manera secuencial, es decir, el servidor atiende al primero y cuando termina con él atiende al siguiente. En el segundo son atendidos al mismo tiempo. Más adelante se describe con más detalle los dos casos.

En cualquiera de estos dos casos se deben realizar previamente los siguientes pasos:

    Creación de un socket.
    Define estructura del socket
    Asociación del socket a una dirección IP y puerto
    Marcar el socket como «pasivo», permitiendo que reciba peticiones de conexión
    Esperar a recibir una petición de conexión de un cliente. Ver apartados siguientes para el código de un servidor secuencial o uno concurrente.

A partir de este punto podemos gestionar cómo atender a los clientes de manera secuencial o concurrente.

En el siguiente ejemplo se verá cómo realizar estos tres pasos:

#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>

/* configuracion del servidor, valores hardcoded */
#define SERV_PORT   8080               /* Puerto */
#define SERV_HOST_ADDR "192.168.0.21"/* IP */ 
#define BACKLOG   5         /* conexiones en cola */

int main(int argc, char* argv[])                
{ 
    int sockfd, connfd ;               /* descriptores de archivo */
    unsigned int len;                    /* tamano estructura cliente */
    struct sockaddr_in servaddr, client;   /* estructura para sockets */

    /* 1.- Crea el socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1)  /* chequea error */
    { 
        return -1;
    }

    /* 2 . Define la estructura del socket */ 
    memset(&servaddr, 0, sizeof(servaddr));   /* inicializa con 0 */
    servaddr.sin_family  = AF_INET; 
    servaddr.sin_addr.s_addr   = inet_addr(SERV_HOST_ADDR); 
    servaddr.sin_port  = htons(SERV_PORT); 
           
    /* 3.- Asocia el socket creado a la dirección IP y el puerto */
   if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        return -1;
    } 
  
    /* 4.- Marca el socket como pasitvo permitiendo la entrada de conexiones*/
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        return -1;
    } 
   
  /* Aqui sigue el codigo de servidor secuencial o concurrente segun diseno */

Servidor Iterativo (secuencial)

El servidor atiende a un cliente y cuando ha terminado con él, atiende al siguiente cliente. Esta opción es adecuada cuando cada cliente se atiende de manera muy rápida.

La llamada al sistema accept crea un nuevo socket para el cliente que va a atender, y cuando el servidor cierre el descriptor de socket correspondiente, volverá a llamar a la función accept para esperar a que un nuevo cliente trate de conectarse y creará entonces un nuevo socket.

Servidor Concurrente

El servidor puede atender a varios clientes simultáneamente. Un método tradicional de programarlo es crear un proceso hijo para cada nuevo cliente. También se podría crear un hilo para cada uno.

Ejemplo servidor iterativo.

/* ... */
len = sizeof(client); 

while(1)
{
    connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
    if (connfd < 0) 
    { 
       /* maneja error */
    }
    else
    {
       /* comunicación con el cliente */
    }
}

    La llamada al sistema accept (…) bloquea el programa esperando a que un cliente trate de conectarse a él. (*)
    En el momento en que llega la primera petición se crea un nuevo socket, cuyo file descriptor es connfd.
    Si el connfd tiene un valor de -1 se ha producido un error, es preciso manejarlo.
    Sie connfd tiene otro valor (diferente de -1) el cliente ha sido aceptado y es el valor del descriptor de archivo para el cliente aceptado.
    Con este descriptor de archivo connfd ya pueden realizarse operaciones de lectura y escritura read(…)/write(…)
    Una vez finalizada la comunicación es preciso cerrar ese descriptor de archivo y se puede atender al próximo cliente.
    (*) En caso de que hubiera varios clientes en cola esperando a ser atendidos, la llamada accept aceptaría al primero de la misma. 

Ejemplo Servidor concurrente:

/* ... */
len = sizeof(client); 
while (1)
{
     connfd = accept(sockfd, (struct sockaddr *)&client, &len);  
     /* TODO: chequear connfd es != 0*/
     if (fork() == 0)               /* proceso hijo */
     { 
          close (sockfd);
          
          /* comunicacion con el cliente */
          exit (0);
     }
    else                           /* proceso padre */
    {
        close(connfd);
    }
} 

La llamada al sistema accept (…) bloquea el programa esperando a que un cliente trate de conectarse a él.

    De la misma manera que en el servidor iterativo aplica los pasos 1 a 4.
    con el fork() se crea un nuevo proceso hijo.
    El proceso hijo es el encargado de comunicarse con el cliente. Ya que el proceso hijo hereda los descriptores abiertos por el padre, sockfd, que es el descriptor de archivo del socket que escucha a las peticiones entrantes debe ser cerrado. Una vez termine el proceso hijo su comunicación con el cliente debe terminar el proceso.
    El proceso padre cierra el descriptor del nuevo socket connfd ya que es el hijo quien debe comunicarse con el mismo.
    El padre vuelve a iterar el bucle y a llamar a accept para atender a otro cliente de manera concurrente o esperar a que haya una petición de conexión. 