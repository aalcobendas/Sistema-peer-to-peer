#include <stdio.h>
#include <stdlib.h>

struct user{
  char user[256];
  char IP[256];
  char port[256];
};


int registrar (char *nombre);
int dar_de_baja (char *nombre);
int conectar (char *nombre, char* dir_IP, char* puerto);
int publicar (char *nombre, char *file, char* descripcion);
int borrar (char *nombre, char *file);
int lista_usuarios(char *nombre, struct user *lista);
int num_usuarios(char *nombre, int *v);
int desconectar(char *nombre);
int num_ficheros(char *nombre, char *usuario, int *v);
int list_content(char *usuario,  char **contenido);
