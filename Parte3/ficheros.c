#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "ficheros.h"


const int TAM_LINEA = 250;

int registrar (char *nombre){
  struct stat st = {0};
  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre);


  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt


  FILE *fichero;


  if(stat("./Almacenamiento", &st) == -1){
    mkdir("./Almacenamiento", 0700);
  }

  if (stat("./Almacenamiento/usuarios", &st) == -1) { //primer usuario en registratse
      mkdir("./Almacenamiento/usuarios", 0700);
      printf("Creo la carpeta 'usuarios'.\n");

      if (stat(USUARIO, &st) == -1){ //usuario no registrado
        mkdir(USUARIO, 0700);
        if (stat("./Almacenamiento/usuariosConectados", &st) == -1) {
            mkdir("./Almacenamiento/usuariosConectados", 0700); //primer usuario en conectarse
            //printf("Creo la carpeta 'usuariosConectados'.\n");
        } //ya existe usuariosConectados, hay mas usuarios conectados

        fichero= fopen(USUARIO_CONECTADO, "wt+"); //usuario nuevo, creo su fichero
        if(fichero != NULL){ //se crea el fichero
          fclose(fichero);
          printf("Fichero creado correctamente. \n");
          return 0;
        }else{
          fclose(fichero);
          printf("Hubo un error al crear el fichero.\n");
          return 2;
        }
        printf("Usuario registrado correctamente.\n");
        return 0;
    }else{ //usuario ya registrado, no va a ocurrir porque la carpeta no existia.
        printf("El usuario ya esta registrado.\n");
        return 1;
    }
  }else{ //ya hay usuarios registrados
    if (stat(USUARIO, &st) == -1){ //usuario nuevo
         mkdir(USUARIO, 0700);

         fichero= fopen(USUARIO_CONECTADO, "wt+"); //usuario nuevo, creo su fichero
         if(fichero != NULL){ //se crea el fichero
           fclose(fichero);
           printf("Usuario registrado correctamente.\n");
           return 0;
         }else{
           fclose(fichero);
           printf("Hubo un error al crear el fichero.\n");
           return 2;
         }

       }else{ //usuario ya registrado
         printf("El usuario ya esta registrado.\n");
         return 1;
       }

  }
  return 2;
}

//UNREGISTER
int dar_de_baja (char *nombre){
  struct stat st = {0};
  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre); // Usuarios = ruta + nombre

  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt


  if (stat("./Almacenamiento/usuarios", &st) == -1) { //no hay usuarios registrados
      printf("No hay usuarios registrados.\n");
      return 2;
  }else{ //hay usuarios registrados
    if (stat(USUARIO, &st) == -1){ //el usuario no esta registrado
      printf("El usuario no esta registrado.\n");
      return 1;
    }else{ //el usuario esta registrado
      rmdir(USUARIO); //Se borra el directorio del usuario.
      remove(USUARIO_CONECTADO); //borro su fichero
      printf("El usuario ha sido borrado con exito.\n");
      return 0;
    }
    return 2; //cualquier otro caso

  }
  return 2; //cualquier otro caso

}

//Se tiene que crear la carpeta usuariosConectados y por cada usuario registrado un fichero con IP PUERTO
int conectar (char *nombre, char* dir_IP, char* puerto){
  struct stat st = {0};
  char USUARIO[TAM_LINEA];
  char USR_REG[TAM_LINEA];

  strcpy(USR_REG, "./Almacenamiento/usuarios/");
  strcat(USR_REG, nombre);

  strcpy(USUARIO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO, nombre);
  strcat(USUARIO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt

  FILE *fichero;
  printf("%s\n",USR_REG);

    //compruebo que el usurio esta registrado
  if(stat(USR_REG, &st) == -1){
    printf("Usuario no registrado. Por favor, registrese.\n");
    return 1;
  }else{ //el usuario esta registrado, el fichero ya existe
    printf("El usuario esta registrado.\n");
    fichero= fopen(USUARIO, "r"); //abro en modo lectura

    if(fichero != NULL){
        //leo el fichero para comprobar si el usuario ya esta conectado
      fseek(fichero, 0, SEEK_END);
      long int res = ftell(fichero);
      printf("%ld\n", res);

      if(res != 0){
        fclose(fichero);
        printf("Usuario ya conectado.\n");
        return 2;
      }

        //escribo IP - puerto
        char IPpuerto[256]= "";
        strcat(IPpuerto,dir_IP);
        strcat(IPpuerto," ");
        strcat(IPpuerto,puerto);

        printf("IPpuerto: %s\n",IPpuerto);
        fichero= fopen(USUARIO, "w+"); //abro en modo lectura
        if(fputs(IPpuerto, fichero) <0){
          printf("Hubo un error al escribir el archivo.\n");
          fclose(fichero);
          return 3; //hubo un error al escribir el archivo
        }
        //archivo escrito correctamente
        fclose(fichero);
        //printf("Fichero creado correctamente. \n");
        return 0;
    }

  }
  return 3;

}

int publicar (char *nombre, char *file, char* descripcion){
  struct stat st = {0};
  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre); // Usuarios = ruta + nombre

  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt

  char PUBLICACION[TAM_LINEA];
  strcpy(PUBLICACION,USUARIO);
  strcat(PUBLICACION,"/");
  strcat(PUBLICACION, file);
  strcat(PUBLICACION, ".txt"); //./Almacenamiento/usuarios/nombre/file.txt

  FILE *fichero_user;
  FILE *fichero_publicacion;

if (stat(USUARIO, &st) == -1) { //el usuario no esta registrado
  printf("Usuario no no existe.\n");
  return 1;
}else{
    fichero_user= fopen(USUARIO_CONECTADO, "r"); //abro en modo lectura
    fseek(fichero_user, 0, SEEK_END);
    long int res = ftell(fichero_user);
    if(res == 0){
      fclose(fichero_user);
      printf("Usuario NO conectado.\n");
      return 2;
    }
    fclose(fichero_user); //cierro el fichero del usuario

    //USUARIO CONECTADO

    if (stat(PUBLICACION, &st) == -1) {
      printf("hola");
      fichero_publicacion= fopen(PUBLICACION, "wt+"); //abro en modo creacion/escritura
      if(fichero_publicacion != NULL){
        char archivo[256]= "";
        strcpy(archivo,descripcion);

        printf("Descripcion del archivo: %s\n",archivo);
        if(fputs(archivo, fichero_publicacion)<0){
          printf("Hubo un error al escribir el archivo.\n");
          fclose(fichero_publicacion);
          return 4; //hubo un error al escribir el archivo
        }
        //archivo escrito correctamente
        fclose(fichero_publicacion);
        printf("Fichero creado correctamente. \n");
        return 0;
      }
    }else{
      printf("Fichero ya publicado.\n");
      return 3;
    }
}
return 4;
}

int borrar (char *nombre, char *file){
  struct stat st = {0};
  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre); // Usuarios = ruta + nombre

  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt

  char PUBLICACION[TAM_LINEA];
  strcpy(PUBLICACION,USUARIO);
  strcat(PUBLICACION,"/");
  strcat(PUBLICACION,file);
//  strcat(PUBLICACION,".txt"); //./Almacenamiento/usuarios/nombre/file.txt

  FILE *fichero;
  FILE *fichero_user;
if (stat(USUARIO, &st) == -1) { //el usuario no esta registrado
  printf("Usuario no no existe.\n");
  return 1;
}else{
  fichero_user= fopen(USUARIO_CONECTADO, "r"); //abro en modo lectura
  fseek(fichero_user, 0, SEEK_END);
  long int res = ftell(fichero_user);
  if(res == 0){
    fclose(fichero_user);
    printf("Usuario NO conectado.\n");
    return 2;
  }else{ //usuario conectado
    fichero= fopen(PUBLICACION, "r");
    if(fichero != NULL){
      remove (PUBLICACION);
      fclose(fichero);
      printf("Fichero borrado correctamente. \n");
      return 0;
    }else{
      printf("El fichero no existe\n");
      return 3;
    }
  }
}
return 4;

}

int num_usuarios(char *nombre, int *v){
  DIR *dip;
  struct dirent *dit;
  FILE *fichero;
  int i = 0;
  struct stat st = {0};

  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre); // Usuarios = ruta + nombre

  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt

  if (stat(USUARIO, &st) == -1) { //el usuario no esta registrado
    printf("Usuario no no existe.\n");
    return 1;
  }else{

    fichero= fopen(USUARIO_CONECTADO, "r"); //abro en modo creacion/escritura
    if(fichero != NULL){
      fseek(fichero, 0, SEEK_END);       //leo el fichero para comprobar si el usuario ya esta conectado
      long int res = ftell(fichero);
      /*printf("%s\n",USUARIO_CONECTADO);
      printf("%ld\n", res);*/
      if(res == 0){
        fclose(fichero);
        printf("Usuario no conectado.\n");
        return 2;
      }
      fclose(fichero);
      //usuario conectado
      dip = opendir( "./Almacenamiento/usuariosConectados");
      char USR[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
      strcpy(USR, "./Almacenamiento/usuariosConectados/");

      while ((dit = readdir(dip)) != NULL){
        if(strcmp(dit->d_name, ".") !=0 && strcmp(dit->d_name, "..") !=0){
          strcat(USR,dit->d_name);
          FILE *conectado; //añadimos solo usuarios conectados
          conectado= fopen(USR, "r"); //abro en modo creacion/escritura
          //printf("%s\n", USR);
          fseek(fichero, 0, SEEK_END);       //leo el fichero para comprobar si el usuario ya esta conectado
          long int res = ftell(fichero);
          if(res != 0){
            char file[1024]="";
            strncpy(file, dit->d_name, strlen(dit->d_name)-4); //file contiene el nombre del usuario
            i++;
          }
          strcpy(USR, "./Almacenamiento/usuariosConectados/");
          fclose(conectado);
        }
      }//FIN WHILE
      if (closedir(dip) == -1){
          printf("closedir");
          return 3;
      }
    }else{
      printf("Usuario no conectado.");
      return 2;
    }
    v[0]=i;
    //printf("\n\nreaddir() found a total of %i files\n", i);
    //snprintf(v, 2,"%d",i);
    //printf("%s\n", v);
    return 0;
  }
  return 3;
}

int lista_usuarios(char *nombre, struct user *lista){
  DIR *dip;
  struct dirent *dit;
  char USR[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USR, "./Almacenamiento/usuariosConectados/");
  int i=0;
  dip = opendir( "./Almacenamiento/usuariosConectados");

  while ((dit = readdir(dip)) != NULL){
    if(strcmp(dit->d_name, ".") !=0 && strcmp(dit->d_name, "..") !=0){
      char file[1024]="";
      strncpy(file, dit->d_name, strlen(dit->d_name)-4); //file contiene el nombre del usuario
      FILE *fich;
      strcat(USR, file);    //consulto en el fichero IP-puerto del usuario y se lo concateno a files
      strcat(USR, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt

      fich= fopen(USR, "r");
      if(fich == NULL){
        return 3;
      }

      char usuario[768]="";
      int counter=0;
      if(fgets(usuario, 768, fich)){
        //  printf("%s\n", usuario); //imprime ip y puerto del usuarios
        strcat(file," ");
        strcat(file,usuario);
        char * token = strtok (file, " "); // file = usuario IP puerto

        while(token != NULL){
        //  printf("%s\n", token);
        //  printf("%d\n", i);

          if(counter==0)strcpy(lista[i].user, token);
          if(counter==1)strcpy(lista[i].IP, token);
          if(counter==2){
            strcpy(lista[i].port, token);
            i++;
          }
          counter++;
          token = strtok(NULL, " ");
        }
      }
      strcpy(USR, "./Almacenamiento/usuariosConectados/");
      //guardo en la lista el string.
      fclose(fich);
    }
  }//FIN WHILE


  /*for (int j=0; j<5; j++){
    printf(" USUARIO %s\n",lista[j].user);
    printf(" IP %s\n",lista[j].IP);
    printf(" PUERTO %s\n",lista[j].port);
  }*/

  if (closedir(dip) == -1){
    printf("closedir");
    return 3;
  }
  printf("adios\n");
  return 0;
} //fin lista



int desconectar(char *nombre){
  struct stat st = {0};
  FILE *fichero;
  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre); // Usuarios = ruta + nombre

  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt

  if (stat(USUARIO, &st) == -1) { //el usuario no esta registrado
    printf("Usuario no existe.\n");
    return 1;
  }else{ //usuario registrado
    fichero= fopen(USUARIO_CONECTADO, "r");
    fseek(fichero, 0, SEEK_END);       //leo el fichero para comprobar si el usuario ya esta conectado
    long int res = ftell(fichero);
    if (res==0) { //el usuario no esta conectado
      fclose(fichero);
      printf("Usuario no conectado.\n");
      return 2;
    }else{
      fclose(fichero);
      fichero= fopen(USUARIO_CONECTADO, "w"); //vacio el fichero
      if(fichero != NULL){
        fclose(fichero);
        printf("Usuario desconectado. \n");
        return 0;
      }else{
        fclose(fichero);
        return 3;
      }
    }

  }
  return 3; //hubo un problema
}



int num_ficheros(char *nombre, char *usuario, int *v){
  struct stat st = {0};
  DIR *dip;
  struct dirent *dit;
  FILE *fichero;
  int i=0;
  char USUARIO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO, "./Almacenamiento/usuarios/");
  strcat(USUARIO, nombre); // Usuarios = ruta + nombre

  char USUARIO_CONTENIDO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONTENIDO, "./Almacenamiento/usuarios/");
  strcat(USUARIO_CONTENIDO, usuario); // Usuarios = ruta + nombre

  char USUARIO_CONECTADO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONECTADO, "./Almacenamiento/usuariosConectados/");
  strcat(USUARIO_CONECTADO, nombre);
  strcat(USUARIO_CONECTADO, ".txt"); //./Almacenamiento/usuariosConectados/nombre.txt


  if (stat(USUARIO, &st) == -1) { //el usuario no esta registrado
    printf("El usuario que realiza la operación no existe.\n");
    return 1;
  }else{ //usuario registrado
    fichero= fopen(USUARIO_CONECTADO, "r");
    fseek(fichero, 0, SEEK_END);       //leo el fichero para comprobar si el usuario ya esta conectado
    long int res = ftell(fichero);
    if (res==0) { //el usuario no esta conectado
      fclose(fichero);
      printf("Usuario no conectado.\n");
      return 2;
    }
    fclose(fichero);
    if (stat(USUARIO_CONTENIDO, &st) == -1) { //el usuario no esta registrado
      printf("El usuario del que se quiere conocer su contenido no existe.\n");
      return 3;
    }

    dip = opendir( USUARIO_CONTENIDO);
    i=0;
    while ((dit = readdir(dip)) != NULL){
      if(strcmp(dit->d_name, ".") !=0 && strcmp(dit->d_name, "..") !=0){
          printf("%s\n",dit->d_name);
          i++;
      }
    }


    if (closedir(dip) == -1){
      printf("closedir");
      return 4;
    }
    v[0]=i;
    printf("ficheros.c: Encuentro %d ficheros\n", v[0]);
    return 0;
  }//fin if usr registrado
  return 4;
}


int list_content(char *usuario,  char **contenido){
  DIR *dip;
  struct dirent *dit;
  int i=0;

  char USUARIO_CONTENIDO[TAM_LINEA]; //variable que contiene la ruta para concatenar el nombre
  strcpy(USUARIO_CONTENIDO, "./Almacenamiento/usuarios/");
  strcat(USUARIO_CONTENIDO, usuario); // Usuarios = ruta + nombre

  dip = opendir( USUARIO_CONTENIDO);
  while ((dit = readdir(dip)) != NULL){
    if(strcmp(dit->d_name, ".") !=0 && strcmp(dit->d_name, "..") !=0){
        strcpy((contenido[i]), dit->d_name);
        printf("ficheros.c: %s\n",(contenido[i]));
        i++;
    }
  }

  if (closedir(dip) == -1){
    printf("closedir");
    return 4;
  }
  return 0;

}
