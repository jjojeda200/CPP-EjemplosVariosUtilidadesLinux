# José Juan Ojeda Granados
# CPP-EjemplosVariosRedesLinux
# Ejercicios básicos diversos en CPP (Para linux)

20-12-2021

RedesVarios00.cpp: Detalles manejo de Red

Big Endian:
El byte de más peso (Most Signficiante Byte) es almacenado en la dirección más
baja de memoria

Little Endian:
El byte de menos peso (Least Significant Byte) es almacenado en la dirección de
memoria más baja de memoria.

    struct sockaddr_in
    {
        short            sin_family;    // Ejemplo: AF_INET (IPv4)
        unsigned short   sin_port;      // Puerto, ejemplo: 3490
        struct in_addr   sin_addr;      // struct incrustada in_addr
                                 .s_addr;
        char             sin_zero[8];
    };

// Dirección Internet IPv4
    struct in_addr
    {
        unsigned long s_addr;           // struct - Dirección tratada con inet_aton()
    };

    struct in_addr
    {
        uint32_t s_addr;                // address in network byte order
    };


23-12-2021

RedesVarios01.cpp: Más detalles manejo de Red

22-12-2021

Servidor01.cpp: Programando socket... Ya era hora ;-)

22-12-2021

Cliente01.cpp
