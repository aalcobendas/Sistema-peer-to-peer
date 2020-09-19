#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<arpa/inet.h>
#include <pthread.h>
#include "lines.h"
#include <stdbool.h>
#include "ficheros.h"


#define MAX_LINE 	256

int n;

pthread_attr_t t_attr;
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t thid;
bool busy = true;
struct sockaddr_in server_addr, client_addr; //direcciones de sokets en AF_INET

int tratar_peticion(int *s);

void print_usage() {
	    printf("Usage: server -p puerto \n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char port[256]= "";

	while ((option = getopt(argc, argv,"p:")) != -1) {
		switch (option) {
		    	case 'p' :
				strcpy(port, optarg);
		    		break;
		    	default:
				print_usage();
		    		exit(-1);
		    }
	}
	if (strcmp(port,"")==0){
		print_usage();
		exit(-1);
	}

	printf("Port: %s\n", port);

	//  INSERT SERVER CODE HERE
	int sd, sc; //descriptores de la conexion
	int val;
	int err;

	int size;

  sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Abrimos socket TCP
  if (sd < 0) {
    perror("Error en socket");
    exit(1);
  }

  val = 1;
  err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
  if (err < 0) {
    perror("Error en opction");
    exit(1);
  }

	// completar .....
	//rellenamos la estructura del servidor
	bzero((char *)&server_addr, sizeof(server_addr)); //llenamos la estructura de 0's
	server_addr.sin_family = AF_INET; //dominio AF_INET
	server_addr.sin_addr.s_addr = INADDR_ANY; //direccion del host. Inaddre_any elige cualquiera de la maquina.
	server_addr.sin_port = htons(atoi(port));  //puerto

	printf("init server %d :%s\n ",server_addr.sin_addr.s_addr, port);

	if(bind(sd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { //asignamos un puerto al socket
		printf("Error al asociar el puerto a la conexion\n");
    close(sd);
	  return 1;
  }

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_attr_init(&t_attr);
	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

	listen(sd, SOMAXCONN); //se escucha el puerto
	size = sizeof(client_addr);

	while (1){
		printf("s> \n");

    //se mantiene a la espera de aceptar clientes
		sc = accept(sd, (struct sockaddr *)&client_addr, (socklen_t *)&size);  //se obtiene de aqui la IP
		if(sc ==-1){
			printf("Error en el accept");
			return -1;
		}

	  if(pthread_create(&thid, &t_attr, (void*) tratar_peticion, &sc) == -1){
	    printf("Error al crear el thread\n");
			close(sc);
	    return -1;
	  }

	  /* esperar a que el hijo copie el descriptor */
		pthread_mutex_lock(&mutex);
		while(busy == true)
		   pthread_cond_wait(&cond, &mutex);
		busy = true;
		pthread_mutex_unlock(&mutex);

	}// fin while
	close(sc);
	close (sd);

	return 0;
}// fin main

int tratar_peticion(int *s){
  int sc_local;
	int32_t res;
	char buffer[MAX_LINE];

  pthread_mutex_lock(&mutex); //seccion critica
	  sc_local = *s;
	  busy = false;
	  pthread_cond_signal(&cond); //se envia la señal de activacion
  pthread_mutex_unlock(&mutex);


		n = readLine(sc_local, buffer, MAX_LINE); //leo la operacion
		if(n == -1) return -1;

		printf("Operacion: %s\n", buffer);

		if(strcmp(buffer,"QUIT")==0){
			close(sc_local);
			return 0;
		}else if(strcmp(buffer,"REGISTER")==0){
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			res = registrar(buffer);
			write(sc_local, &res, sizeof(int32_t));
			//printf("hola\n");
			close(sc_local);


		}else if(strcmp(buffer,"UNREGISTER")==0){ //UNREGISTER
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			res = dar_de_baja(buffer);
			write(sc_local, &res, sizeof(int32_t));
			//printf("hola\n");
			close(sc_local);


		}else if(strcmp(buffer,"CONNECT")==0){ //CONNECT
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			char nombre[256];
			strcpy(nombre, buffer);

			struct sockaddr_in addr;
		  socklen_t addr_size = sizeof(struct sockaddr_in);
		  int a = getpeername(sc_local, (struct sockaddr *)&addr, &addr_size);
			if(a==-1) return -1;
			char IP_add[50];
			printf("IP: %s\n", inet_ntoa(addr.sin_addr)); //EJ: 127.0.0.1
		  strcpy(IP_add, inet_ntoa(addr.sin_addr));

			n = readLine(sc_local, buffer, MAX_LINE); //leo el puerto
			char Puerto[7];
			strcpy(Puerto, buffer);

			res = conectar(nombre, IP_add, Puerto);
			write(sc_local, &res, sizeof(int32_t));
			//printf("hola\n");
			close(sc_local);

		}else if(strcmp(buffer,"DISCONNECT")==0){ //DISCONNECT
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			res = desconectar(buffer);
			write(sc_local, &res, sizeof(int32_t));
			//printf("hola\n");
			close(sc_local);


		}else if(strcmp(buffer,"PUBLISH")==0){ //PUBLISH
			char nombre[256];
			char file[256];
			char descripcion[256];
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			strcpy(nombre, buffer);

			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre del fichero
			if(n == -1) return -1;

			strcpy(file, buffer);

			n = readLine(sc_local, buffer, MAX_LINE); //leo la descripcion del fichero
			if(n == -1) return -1;

			strcpy(descripcion, buffer);
			printf("voy a publicar\n");
			res = publicar(nombre,file,descripcion);
			printf("Cod error: %d\n", res);
			write(sc_local, &res, sizeof(int32_t));
			close(sc_local);


		}else if(strcmp(buffer,"DELETE")==0){ //DELETE
			char nombre[256];
			char file[256];
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			strcpy(nombre, buffer);

			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre del fichero
			if(n == -1) return -1;

			strcpy(file, buffer);
			res = borrar(nombre,file);
			write(sc_local, &res, sizeof(int32_t));
			close(sc_local);


		}else if(strcmp(buffer,"LIST_USERS")==0){ //LIST USERS
			 int *p1 = malloc(1*sizeof(int)); //numero de elementos recibidos
			char nombre[256];
			n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre
			if(n == -1) return -1;
			strcpy(nombre, buffer);

			res =  num_usuarios(nombre, p1);
			if(res == 0){
				struct user *lista;
				lista = (struct user *) malloc((*p1)*(sizeof(struct user)));
				res= lista_usuarios(nombre, lista);

				//GENERAR STRING

				char resultado[15];
				sprintf(resultado, "%d", res);

				char num_usr[15];
				sprintf(num_usr, "%d", (*p1));
				//printf("ENVIO %s\n",num_usr);
				enviar(sc_local, resultado, strlen(resultado)+1);
				//printf("RESULT %s\n",resultado);
				printf("ENVIO %s\n",num_usr);
				enviar(sc_local, num_usr, strlen(num_usr)+1);

				for (int i=0; i<(*p1); i++){
					char imprimir[250]="";
					strcpy(imprimir,lista[i].user);
					enviar(sc_local, imprimir, strlen(imprimir)+1);
					strcpy(imprimir,lista[i].IP);
					enviar(sc_local, imprimir, strlen(imprimir)+1);
					strcpy(imprimir,lista[i].port);
					printf("%s\n", imprimir);
					enviar(sc_local, imprimir, strlen(imprimir)+1);

				}
				//enviar lista_usuarios
				free(lista);
				free(p1);
			}else{
				//no se envia la lista
				free(p1);
				char resultado[15];
				sprintf(resultado, "%d", res);
				enviar(sc_local, resultado, strlen(resultado)+1);
			}

			printf("hola\n");
			close(sc_local);

	  	pthread_exit(0);

		}else if(strcmp(buffer,"LIST_CONTENT")==0){ //LIST CONTENT

		 char nombre[256];
		 n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre mio
		 if(n == -1) return -1;
		 strcpy(nombre, buffer);

		 char usuario[256];
		 n = readLine(sc_local, buffer, MAX_LINE); //leo el nombre del otro
		 if(n == -1) return -1;
		 strcpy(usuario, buffer);

		 int p1;
		 res =  num_ficheros(nombre, usuario, &p1);
		 printf("server.c: Resultado: %d, Nº Ficheros : %d\n", res , p1);

		 if(res == 0){
			 char **contenido;
			 contenido = (char**) malloc((p1)*sizeof(char*));  // creáis primero el array de punteros a char
			 for (int j = 0; j<(p1); j++) contenido[j]= (char*)malloc(256);	 // ahora reservas cada posición del vector de cadenas


			 res= list_content(usuario, contenido);


			 if(res == 0){
				 char imprimir[256];
				 char num_usr[15];
				 char resultado[15];
				 sprintf(resultado, "%d", res);
				 enviar(sc_local, resultado, strlen(resultado)+1);
				 sprintf(num_usr, "%d", p1);
				 enviar(sc_local, num_usr, strlen(num_usr)+1);
				 printf("server.c: ENVIO %s\n",num_usr);

			  for(int i=0; i<(p1); i++){
					strcpy(imprimir,(contenido[i]));
					printf("server.c: %s\n",imprimir);
					enviar(sc_local, imprimir, strlen(imprimir)+1);
				}

			 }

			 for (int k = 0; k <(p1); k++) free(contenido[k]);
			 free(contenido);
		 }else{ 			 //no se envia la lista
			 char resultado[15];
			 sprintf(resultado, "%d", res);
			 enviar(sc_local, resultado, strlen(resultado)+1);
		 }
		 close(sc_local);
		 pthread_exit(0);

		}else{ //operacion invalida

		}

  pthread_exit(0);

  return(0);
}
