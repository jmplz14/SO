#include<dirent.h>
//#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include <unistd.h>
//#include <errno.h>
void recorrerDir(const char *ruta, int* num_ficheros, long* ocupado, char *camino){
	char anterior[1024];
	
	DIR *directorio;
	struct dirent *datos;
	struct stat metadatos;
	if((directorio = opendir(ruta)) == NULL){
		printf("\nError al intentar abir el directorio %s",ruta);
		perror("\nError en opendir\n");
	}else{

		getcwd(anterior, sizeof(anterior));
		chdir(ruta);
		while((datos=readdir(directorio)) != NULL){
			char camino_nuevo[1024];
			if ( strcmp(datos->d_name, ".") != 0 && strcmp(datos->d_name, "..") != 0 ){
            
				if(lstat(datos->d_name,&metadatos) < 0){
						printf("\nError al intentar acceder a los atributos dearchivo de %s\n", datos->d_name); 
						perror("\nError en lstat");
				}else{
					if(S_ISREG(metadatos.st_mode)){
						if((metadatos.st_mode & (S_IXGRP|S_IXOTH)) == (S_IXGRP|S_IXOTH)){
							printf("%s/%s %ld\n",camino,datos->d_name, datos->d_ino);
							*num_ficheros += 1;
							*ocupado += metadatos.st_size;
						}
					}else if(S_ISDIR(metadatos.st_mode)){
						strcpy(camino_nuevo, camino);
						strcat(camino_nuevo, "/");
						strcat(camino_nuevo, datos->d_name);
						recorrerDir(datos->d_name,num_ficheros,ocupado,camino_nuevo);
					}
				}
			
			} 
		}
		chdir(anterior);
				
	}
		
}
int main(int argc, char *argv[]){
	int num_ficheros = 0;
	long ocupado = 0;
	if(argc != 2) {
		printf("\nSintaxis de ejecucion \n");
		exit(EXIT_FAILURE);
	}
	recorrerDir(argv[1],&num_ficheros,&ocupado,argv[1]);
	printf("Existen %d archivos con permosos x para grupo y otros\n",num_ficheros);
	printf("El tamaño ocupado por estos ficheros es de  %ld bytes\n",ocupado);
	return EXIT_SUCCESS;
}
