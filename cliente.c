/*******************************************************
 * Protocolos de Transporte
 * Grado en Ingeniería Telemática
 * Dpto. Ingeníería de Telecomunicación
 * Univerisdad de Jaén
 *
 *******************************************************
 * Práctica 1.
 * Fichero: cliente.c
 * Versión: 3.1
 * Fecha: 10/2020
 * Descripción:
 * 	Cliente sencillo TCP para IPv4 e IPv6
 * Autor: Juan Carlos Cuevas Martínez
 *
 ******************************************************
 * Alumno 1: Jose Antonio Barrios Garcia
 * Alumno 2: David Antonio Barrios Garcia
 *
 ******************************************************/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>		// Biblioteca estándar de entrada y salida
#include <ws2tcpip.h>	// Necesaria para las funciones IPv6
#include <conio.h>		// Biblioteca de entrada salida básica
#include <locale.h>		// Para establecer el idioma de la codificación de texto, números, etc.
#include "protocol.h"	//Declarar constantes y funciones de la práctica

#pragma comment(lib, "Ws2_32.lib")//Inserta en la vinculación (linking) la biblioteca Ws2_32.lib
//probamos 

int main(int* argc, char* argv[])
{
	SOCKET sockfd;
	struct sockaddr* server_in = NULL;
	struct sockaddr_in server_in4;
	struct sockaddr_in6 server_in6;
	int address_size = sizeof(server_in4);
	char buffer_in[1024], buffer_out[1024], input[1024];
	int recibidos = 0, enviados = 0;
	int estado,recibir = 1;
	int cabecera = 0;
	char option;
	char opcion;
	int ipversion = AF_INET;//IPv4 por defecto
	char ipdest[256];
	char default_ip4[16] = "127.0.0.1";
	char default_ip6[64] = "::1";
	char destino[60], emisor[60];
	char  a[1000];

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	//Inicialización de idioma
	setlocale(LC_ALL, "es-ES");


	//Inicialización Windows sockets - SOLO WINDOWS
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		return(0);
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return(0);
	}
	//Fin: Inicialización Windows sockets

	printf("**************\r\nCLIENTE TCP SENCILLO SOBRE IPv4 o IPv6\r\n*************\r\n");

	do {
		printf("CLIENTE> ¿Qué versión de IP desea usar? 6 para IPv6, 4 para IPv4 [por defecto] ");
		gets_s(ipdest, sizeof(ipdest));

		if (strcmp(ipdest, "6") == 0) {
			//Si se introduce 6 se empleará IPv6
			ipversion = AF_INET6;

		}
		else { //Distinto de 6 se elige la versión IPv4
			ipversion = AF_INET;
		}

		sockfd = socket(ipversion, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET) {
			printf("CLIENTE> ERROR\r\n");
			exit(-1);
		}
		else {
			printf("CLIENTE> Introduzca la IP destino (pulsar enter para IP por defecto): ");
			gets_s(ipdest, sizeof(ipdest));

			//Dirección por defecto según la familia
			if (strcmp(ipdest, "") == 0 && ipversion == AF_INET)
				strcpy_s(ipdest, sizeof(ipdest), default_ip4);

			if (strcmp(ipdest, "") == 0 && ipversion == AF_INET6)
				strcpy_s(ipdest, sizeof(ipdest), default_ip6);

			if (ipversion == AF_INET) {
				server_in4.sin_family = AF_INET;
				server_in4.sin_port = htons(TCP_SERVICE_PORT);
				inet_pton(ipversion, ipdest, &server_in4.sin_addr.s_addr);
				server_in = (struct sockaddr*)&server_in4;
				address_size = sizeof(server_in4);
			}

			if (ipversion == AF_INET6) {
				memset(&server_in6, 0, sizeof(server_in6));
				server_in6.sin6_family = AF_INET6;
				server_in6.sin6_port = htons(TCP_SERVICE_PORT);
				inet_pton(ipversion, ipdest, &server_in6.sin6_addr);
				server_in = (struct sockaddr*)&server_in6;
				address_size = sizeof(server_in6);
			}

			//Cada nueva conexión establece el estado incial en
			estado = S_HELO;

			if (connect(sockfd, server_in, address_size) == 0) {
				printf("CLIENTE> CONEXION ESTABLECIDA CON %s:%d\r\n", ipdest, TCP_SERVICE_PORT);

				//Inicio de la máquina de estados
				do {
					switch (estado) {
					case S_HELO:
						// Se recibe el mensaje de bienvenida
						break;

					case S_START:
						// establece la conexion de aplicacion 
						printf("SMTP> Introduzca el nombre del host: ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						else {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", HELO, input, CRLF);
						}
						break; // hasta aqui deberia de funcionar bien

					case S_MAIL: //meter esto en el .h tambien se puede poner un reset aqui
						printf("Introducir el correo del remitente : ");
						gets_s(input, sizeof(input));
						strcpy_s(emisor, sizeof(emisor),input);//cargamos los datos por si luego el usuario quiere no introducir de nuevo
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", MAIL, input, CRLF);
						break; //el envio tendria que hacerse correctamente tambien

					case S_RCPT:
						printf("SMTP> Desde que correo le gustaria mandar el mensaje: ");
						gets_s(input, sizeof(input));
						strcpy_s(destino, sizeof(destino),input);
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						else
							sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", RCPT, input, CRLF);
						break; //hasta aqui tambien tendria que estar correcto

					case S_DATA: //Meter el reset por si quiere borrar el mensaje
						cabecera = 0;
						printf("Los datos son correctos¿? : ");
						opcion = _getche();
						if(opcion == 's' || opcion == 'S'){
							sprintf_s(buffer_out,sizeof(buffer_out),"%s%s",DATA, CRLF);
						}
						else{
	//						sprintf(buffer_out, sizeof(buffer_out, "%s%s", RESET, CRLF);
							estado = S_HELO;
						}
						break;

					case S_MENSA:// FALTA DECLARAR ESTE ESTADO TAMBIEN por aqui lo dejamos hoy
						//hacer una maquina de estados en la cual se recorra lo de emisor, receptor y el mensaje y comprobar que acabe en un punto con un condicional
						switch (cabecera) { //realizado en clase
						case 0:
							printf("Introducir un asunto : ");
							gets_s(input, sizeof(input));
							sprintf_s(buffer_out, sizeof(buffer_out), "Subject : %s %s", input,CRLF);
							cabecera++;
							break;
						case 1:
							printf("Introduce un emisor : ");
							gets_s(input, sizeof(input));
							sprintf_s(buffer_out, sizeof(buffer_out), "From : %s %s %s", emisor, input,CRLF); //mandamos por defecto el que hemos cargado arriba + el que quiera añadir
							cabecera++;
							break;
						case 2:
							printf("Introduce un receptor : ");
							gets_s(input, sizeof(input));
							sprintf_s(buffer_out, sizeof(buffer_out), "To : %s %s %s", destino, input,CRLF); //mandamos por defecto el que hemos cargado arrib + alguno mas
							cabecera++;
							break;
						case 3:
							printf("SMTP [Escribe un mensaje y pulse '.' para salir]  : ");
							gets_s(input, sizeof(input));
							strcpy_s(a, sizeof(a), input); //por si los necesitamos mas adelante los guardamos tambien
							if (strcmp(input, ".") == 0) {
								sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", input, CRLF);
								estado = S_FINAL; // cambiar estado por el nuevo
							}
							else
								sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", input, CRLF);
							break;
						}

					}

					if (estado != S_HELO) {
						enviados = send(sockfd, buffer_out, (int)strlen(buffer_out), 0);
						if (enviados == SOCKET_ERROR) {
							estado = S_QUIT;
							continue;// La sentencia continue hace que la ejecución dentro de un
									 // bucle salte hasta la comprobación del mismo.
						}
					}
					if(estado != S_MENSA){
				//if(recibir == 1){ //comprobamos si ha metido el . sino se repite lo del mensaje
					recibidos = recv(sockfd, buffer_in, 512, 0);
					if (recibidos <= 0) {
						DWORD error = GetLastError();
						if (recibidos < 0) {
							printf("CLIENTE> Error %d en la recepción de datos\r\n", error);
							estado = S_QUIT;
						}
						else {
							printf("CLIENTE> Conexión con el servidor cerrada\r\n");
							estado = S_QUIT;
						}
					}
					else {
						buffer_in[recibidos] = 0x00; // en s char se llena el buffer
						printf(buffer_in);
						switch (estado) {
						case S_HELO:
							if (strncmp(buffer_in, "22", 2) == 0) {
								estado = S_START;
							} //tenemos que añadir el resto de los estados
							break;
						case S_START:
							if(strcmp(buffer_in ,"250", 3) == 0){
								estado = S_MAIL;
							}
							else {
								estado = S_MAIL; //tenemos un fallo aqui me cago en diooo
							}
							break;
						case S_MAIL:
							if (strncmp(buffer_in, "25", 2) == 0) {
							estado=S_RCPT;
							}
							estado = S_RCPT;
							break;
						case S_RCPT:
							if (strncmp(buffer_in, "25", 2) == 0) {
								estado = S_DATA;
							}
							estado = S_DATA;
							break;
						case S_DATA:
							estado=S_MENSA;
							break;
						case S_FINAL:
							printf("Quiere mandar otro mensaje: [s/n] ");
							gets_s(input, sizeof(input));
							if (strcmp(input, "s") == 0) {
								estado = S_MAIL;
							}
							else {
								estado = S_QUIT;
							}
						default:
							if (strncmp(buffer_in, "25", 2) == 0) {
								estado ++;
							}
							break;
						}
						}
				} //final del if recibir == 1;
				} while (estado != S_QUIT);
			}
			else {
				int error_code = GetLastError();
				printf("CLIENTE> ERROR AL CONECTAR CON %s:%d\r\n", ipdest, TCP_SERVICE_PORT);
			}
			closesocket(sockfd);

		}
		printf("-----------------------\r\n\r\nCLIENTE> Volver a conectar (S/N)\r\n");

		option = _getche();

	} while (option != 'n' && option != 'N');

	return(0);
}
