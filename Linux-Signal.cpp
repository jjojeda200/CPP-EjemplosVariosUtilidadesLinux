/*
    José Juan Ojeda Granados, 03-01-2022
    signal manejo de señales, programar la función Handler que atienda a la señal

    Comando Kill, ps

    https://man7.org/linux/man-pages/man2/signal.2.html

    https://man7.org/linux/man-pages/man7/signal.7.html

    https://man7.org/linux/man-pages/man2/sigaction.2.html

*/
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void miSignalHandler(int sig)
{
    printf("\n\e[0;34m La señal recibida es:\e[0;37m %d \e[0m", sig);
}

int main(int argc, char *argv[])
{
    system("clear");

    // Capturamos dos señales para que provoque una interrupción en la aplicación
    // sighandler_t signal(int signum, sighandler_t handler);
    signal(SIGIO, &miSignalHandler);
    signal(SIGALRM, &miSignalHandler);  // Esta señal por defecto produce la interrupción

    while (1)
    {
        for (int i = 0; i < 70; i++)
        {
            printf("\e[0;33m.\e[0m");
        }
        sleep(4);
        printf("\e[0;33m\e[0m\n");
    }

    return 0;
}