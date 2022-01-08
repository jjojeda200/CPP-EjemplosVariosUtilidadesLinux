/**
 *	@file	fcsh.cpp
 *	@date 	abril 2007
 *  @author Francisco Charte Ojeda
 *	@brief 	Implementaci�n de la clase FcSh
 */
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "fcsh.hpp"

/** @brief Funci�n auxiliar para generar una matriz char* a partir de un vector STL */
char** StlACpp(vector<string>& Parametros)
{
    char** argv = new char*[Parametros.size()+1];

	unsigned i;
    for (i=0; i<Parametros.size(); i++) {
        argv[i] = new char[Parametros[i].size()+1];
        strncpy(argv[i], Parametros[i].c_str(), Parametros[i].size()+1);
    }
    argv[i] = NULL; // el �ltimo puntero ha de ser NULL para que execvp funcione

	return argv;	
}

/* 
 * Ejecutar
 * 
 * Este m�todo act�a como el nudo central del shell, mostrando el indicador, 
 * solicitando la l�nea de comandos, analiz�ndola y proces�ndola como corresponda.
 * 
 */
int FcSh::Ejecutar()
{
	bool Salir = false;
	
	// Muestro unas breves indicaciones sobre el funcionamiento del int�rprete
	system("clear");
	cout << endl << "Bienvenido a fcsh (Francisco Charte Shell 1.0 8-D)" << endl << endl
	     << "Introduce los comandos a ejecutar como lo har�as habitualmente en Linux," << endl
	     << "separando cada argumento y metacar�cter con espacios." << endl << endl 
	     << "Puedes utilizar los metacaracteres < y > para redireccionar entrada y salida," << endl
	     << "combin�ndolos si interesa, as� como el metacar�cter | para crear una" << endl 
	     << "interconexi�n entre dos procesos. No se pueden combinar < y/o > con |." << endl << endl
	     << "Para salir de fcsh utiliza el comando 'exit' o pulsa Ctrl-C" << endl << endl;
	
	do {
		string Comando, ArchivoIn, ArchivoOut;
		vector<string> Parametros, Pipe;
		
		MostrarPrompt(); // Se muestra el indicador de entrada
		Comando = LeerComando(); // Se recupera una l�nea de comando
		// Se analiza su contenido
		if(AnalizaLineaComandos(Comando, Parametros, ArchivoIn, ArchivoOut, Pipe))
		    // y se procesa el comando 
			Salir = ProcesaComando(Comando, Parametros, ArchivoIn, ArchivoOut, Pipe);
	} while(!Salir);
	
	return 0;
}

/*
 * MostrarPrompt
 * 
 * M�todo encargado de mostrar el indicador del shell.
 * 
 */
void FcSh::MostrarPrompt()
{
	cout << "[" << setw(3) << ++_nComando << "] -> ";
}

/*
 * LeerComando
 * 
 * Este m�todo recupera de la entrada est�ndar la l�nea de comandos a ejecutar, que
 * devuelve como resultado en un string.
 * 
 */
string FcSh::LeerComando()
{
	string Linea;
	
	getline(cin, Linea);
	return Linea;
}

/*
 * AnalizaLineaComandos
 * 
 * Este m�todo toma como entrada la l�nea de comandos completa y un vector de cadenas en
 * el que se devolver� la lista de par�metros. El valor de retorno, de tipo bool, indica
 * si se ha introducido algo o la l�nea de comandos estaba vac�a.
 * 
 */
bool FcSh::AnalizaLineaComandos(string& Comando, vector<string>& Parametros, string& ArchivoIn, string& ArchivoOut, vector<string>& Pipe)
{
	stringstream Entrada(Comando); // Tratamos la cadena de entrada como un flujo o stream
	string Elemento;
	vector<string>* Destino = &Parametros;
		
	while(Entrada >> Elemento) // Vamos obteniendo las palabras de la cadena
		// Comprobamos la aparici�n de <, > y |
		switch(Elemento[0]) {
			case '<': // Tras el car�cter < estar� el nombre de archivo
			    Entrada >> ArchivoIn;
				break;			    
			case '>':
			    Entrada >> ArchivoOut;
			    break;
			case '|':
			    Destino = &Pipe;
			    break; 
			default:  // por defecto 
	    		Destino->push_back(Elemento); // las almacenamos como elementos individuales del vector
		}
	
	if(Parametros.size())
		Comando = Parametros[0]; // El primer elemento es el ejecutable

	return Parametros.size();
}

/*
 * ProcesaComando
 * 
 * Funci�n encargada de analizar el comando y procesarlo como corresponda
 * 
 */
bool FcSh::ProcesaComando(string Comando, vector<string>& Parametros, string ArchivoIn, string ArchivoOut, vector<string>& Pipe)
{
	// Primero procesar los comandos internos del int�rprete
	if(Comando == "exit") return true;
	
	// No es un comando interno, as� que creo un nuevo proceso o varios, seg�n se precise
	
    // Compruebo si hay una interconexi�n con otro programa
	if(Pipe.size()) {
		int fds[2];
		pipe(fds); // Creo la tuber�a sin nombre para conectar dos procesos
		
		if(!fork()) { // Primer hijo
			close(fds[0]); // Cierro el canal de lectura en la interconexi�n
			dup2(fds[1], STDOUT_FILENO); // y desv�o la salida est�ndar a la escritura en la tuber�a
			
			char** argv = StlACpp(Parametros); // Par�metros correspondientes a este programa
	        // sustituyo el proceso actual por el del comando indicado
	 	    if(execvp(Comando.c_str(), argv) == -1) {
	 	    	// teniendo en cuenta un posible fallo
	 	    	cout << "Fallo al intentar ejecutar " << Comando << endl;
	 	    	exit(-1);
	 	    }
		}
		
		if(!fork()) { // segundo hijo
			close(fds[1]); // Cierro el canal de escritura en la interconexi�n
			dup2(fds[0], STDIN_FILENO); // y desv�o la entrada est�ndar a la lectura de la tuber�a
			
			char** argv = StlACpp(Pipe); // Par�metros correspondientes a este programa
	        // sustituyo el proceso actual por el del comando indicado
	 	    if(execvp(Pipe[0].c_str(), argv) == -1) {
	 	    	// teniendo en cuenta un posible fallo
	 	    	cout << "Fallo al intentar ejecutar " << Pipe[0] << endl;
	 	    	exit(-1);
	 	    }
		}
		
		// El padre cierre la lectura y escritura en la tuber�a y espera
		close(fds[0]);
		close(fds[1]);
		
		wait(NULL);
		wait(NULL);
		
		return false;
	}
	
	// No hay interconexi�n, solamente se ejecuta un programa y se tienen en cuenta redireccionamientos
	int f = fork();
	if(!f) { // Si �ste es el proceso hijo
		
		// Obtengo en una matriz de punteros a char los par�metros
        char** argv = StlACpp(Parametros);
        
		// Compruebo si hay redireccionamiento de entrada
		if(!ArchivoIn.empty()) {
			int fIn = open(ArchivoIn.c_str(), O_RDONLY);
			if(fIn == -1) {
				cout << "Fallo al abrir el archivo " << ArchivoIn << endl;
				exit(-1);
			}
			dup2(fIn, STDIN_FILENO);
		}
		
		// Compruebo si hay redireccionamiento de salida
		if(!ArchivoOut.empty()) {
			int fOut = open(ArchivoOut.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fOut == -1) {
				cout << "Fallo al crear el archivo " << ArchivoOut << endl;
				exit(-1);
			}
			dup2(fOut, STDOUT_FILENO);
		}

        // sustituyo el proceso actual por el del comando indicado
 	    if(execvp(Comando.c_str(), argv) == -1) {
 	    	// teniendo en cuenta un posible fallo
 	    	cout << "Fallo al intentar ejecutar " << Comando << endl;
 	    	exit(-1);
 	    }
	} else { // Si �ste es el proceso padre
		wait(NULL); // esperar a que termine el hijo
	}
		
	return false; // No se quiere salir del int�rprete
}