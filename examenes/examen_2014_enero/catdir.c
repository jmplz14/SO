
//exit
#include <unistd.h>
#include <stdlib.h>
//printf
#include <stdio.h>
//open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//dup 
#include <unistd.h>
//opendir
#include <sys/types.h>
#include <dirent.h>
//comprueba que tenga permiso de escritura tanto el usuario como el grupo
#define S_VALIDO(mode) ((mode & (S_IWUSR|S_IWGRP)) == (S_IWUSR|S_IWGRP))
//strlen
#include <string.h>
int recorrerDirectorio(const char *ruta){
	DIR* directorio;
	struct dirent* entrada_directorio;
	struct stat fichero;
	int fd_fichero,leidos;
	unsigned total_bloques = 0;
	char nombre_fichero[1024], datos[1024];
	if((directorio = opendir(ruta)) == NULL){
		printf("Error al abrir el directorio %s",ruta);
		exit(-1);	
	}
	while((entrada_directorio = readdir(directorio)) != NULL){
		
		if(strcmp(entrada_directorio->d_name, ".") != 0 && strcmp(entrada_directorio->d_name, "..") != 0){
			sprintf(nombre_fichero, "%s/%s",ruta,entrada_directorio->d_name);
			if(stat(nombre_fichero,&fichero) >= 0){
				if(S_ISREG(fichero.st_mode) && S_VALIDO(fichero.st_mode)){
					//printf("%s\n",nombre_fichero);
					total_bloques += fichero.st_blocks;
					
					if((fd_fichero = open(nombre_fichero,O_RDONLY)) < 0 ){
						perror("Error al abrir un fichero");
						exit(-1);
					}
					while((leidos = read(fd_fichero,&datos,1024)) != 0){
						write(1,&datos,leidos);
					}
				}
				
			}
		}
		
	}
	return total_bloques;
}

int main (int argc, char *argv[]){
	int fd_salida, num_bloques = 0;
	int salida;
	char numero[255];
	if(argc != 2 && argc != 3){
		printf("No se pasaron el numero de argumentos valido" );
		exit(-1);
	}else if(argc == 3){
		if((fd_salida=open(argv[2],O_CREAT|O_WRONLY|O_TRUNC, 0660)) == -1){
			printf("Error al abrir el fichero %s",argv[2]);
			exit(-1);
		}
		salida = dup(1);
		close(STDOUT_FILENO);
		dup(fd_salida);
	}
	num_bloques = recorrerDirectorio(argv[1]);
	
	if(argc == 3){
		close(fd_salida);
		dup2(salida, 1);
	}
	//sprintf(numero,"%d",num_bloques);
	//write(1,&numero,strlen(numero));
	write(1,&num_bloques,sizeof(num_bloques));
	//printf("Numero de bloques totales = %d\n", num_bloques);
	
}
