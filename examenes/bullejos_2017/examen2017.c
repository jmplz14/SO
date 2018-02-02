#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
void esperar_hijos(int hijos){
	int estado,i;
	for(i = 0; i < hijos; i++){
		wait(&estado);
	}
}
void recorrerDirectorio(const char* path, const char* destino){
		
		DIR *directorio;
		int salir = 0, num_hijos = 0, num_archivos = 0;
		
		struct dirent* elemento;
		struct stat elemento_datos;
		if((directorio = opendir(path)) == NULL){
			printf("\nError al intentar abir el directorio %s",path);
			perror("\nError en opendir\n");
		}else{
			
			while((elemento = readdir(directorio)) != NULL && salir == 0){
				
				if(strcmp(elemento->d_name, ".") != 0 && strcmp(elemento->d_name, "..") != 0){
					char nombre_origen[100];
					strcpy(nombre_origen, path);
					strcat(nombre_origen, "/");
					strcat(nombre_origen, elemento->d_name);
					
					if(stat(nombre_origen,&elemento_datos) >= 0 ){
						
						char nombre_destino[100];
						strcpy(nombre_destino, destino);
						strcat(nombre_destino, "/");
						strcat(nombre_destino, elemento->d_name);
						//printf("%s \n", nombre_origen);
						if( S_ISREG(elemento_datos.st_mode) ){
							char *ptr_origen,*ptr_destino;
							int fd_origen, fd_destino;
							
							//Se crea el nombre completo del fichero origen
							
							
							if((fd_origen = open(nombre_origen,O_RDONLY)) == -1){
								perror("fallo al abrir origen \n");
								exit(1);
							}
							
							if((fd_destino = open(nombre_destino,O_RDWR|O_CREAT|O_TRUNC,elemento_datos.st_mode)) == -1){
								perror("fallo al abrir destion \n");
								exit(1);
							}
							ftruncate(fd_destino, elemento_datos.st_size);
							
							ptr_origen = (char*) mmap(0, elemento_datos.st_size, PROT_READ, MAP_SHARED, fd_origen, 0);
							if(ptr_origen == MAP_FAILED) {

								perror("Fallo mapeando el archivo de entrnada");
								exit(-1);

							}
							ptr_destino = (char*) mmap(0, elemento_datos.st_size, PROT_WRITE, MAP_SHARED, fd_destino, 0);
							if(ptr_destino == MAP_FAILED) {

								perror("Fallo mapeando el archivo de salida");
								exit(-1);

							}
							

							memcpy(ptr_destino, ptr_origen, elemento_datos.st_size);
							
							munmap(ptr_origen, elemento_datos.st_size);
							munmap(ptr_destino, elemento_datos.st_size);
							
							close(fd_origen);
							close(fd_destino);
							num_archivos++;
						}else if(S_ISDIR(elemento_datos.st_mode)){
							
							mkdir(nombre_destino, elemento_datos.st_mode);
							
							int pid;
							if((pid = fork()) < 0){
								perror("La creacion del hijo fallo\n");
								exit(-1);
							}else if(pid == 0){
								salir = 1;
								recorrerDirectorio(nombre_origen,nombre_destino);
								exit(1);
							}else{
								num_hijos++;
							}
							
						}
					}
				}
					
			}
			if(num_hijos > 0)
				esperar_hijos(num_hijos);
				
			write(1,&num_archivos,sizeof(num_archivos));
			
		}
}
int main (int arc, char *argv[]){
	DIR* directorio_destino = opendir(argv[2]);
	int leidos;
	if(errno == ENOENT){
		mkdir(argv[2], 0760);
	}
	int pid, fd[2], num_ficheros, total_ficheros = 0;
	pipe(fd);
	pid = fork(); 
	if(pid == 0){
		close(fd[0]);
		close(STDOUT_FILENO);
		dup(fd[1]);
		recorrerDirectorio(argv[1],argv[2]);
		close(fd[1]);
		//dup(STDOUT_FILENO);
		//printf("Termino el primer hijo");	
		
	}else{
		wait();
		close(fd[1]);
		while( (leidos = read(fd[0],&num_ficheros, sizeof(num_ficheros))) != 0){
			total_ficheros += num_ficheros;

		}
		printf("Termino el padre con un total de %d ficheros copiados", total_ficheros);
	}
}
