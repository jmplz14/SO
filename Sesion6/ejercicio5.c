#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {

	//Declaracion de variables
	struct stat datos;
	struct flock cerrojo;
	int fd_origen, fd_destino, i;
	char *ptr_origen,*ptr_destino;

	if(argc != 3) {

		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <archivo>\n\n", argv[0]);
		exit(1);

	} 

	if((fd_origen = open(argv[1],O_RDONLY)) == -1){
		printf("fallo al abrir origen %s\n", argv[1]);
		exit(1);
	}
	
	if((fd_destino = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0666)) == -1){
		printf("fallo al abrir desitino %s\n", argv[2]);
		exit(1);
	}
	if(stat(argv[1], &datos)){
		printf("%s", strerror(errno));
		exit(-1);
	}
	
	ftruncate(fd_destino, datos.st_size);
	
	
	ptr_origen = (char*) mmap(0, datos.st_size, PROT_READ, MAP_SHARED, fd_origen, 0);
	if(ptr_origen == MAP_FAILED) {

			perror("Fallo mapeando el archivo de entrada");
			exit(-1);

	}
	ptr_destino = (char*) mmap(0, datos.st_size, PROT_WRITE, MAP_SHARED, fd_destino, 0);
	if(ptr_destino == MAP_FAILED) {

			perror("Fallo mapeando el archivo de entrada");
			exit(-1);

	}		
	memcpy(ptr_destino, ptr_origen, datos.st_size);


	//Liberamos los mapas de memoria
	munmap(ptr_origen, datos.st_size);
	munmap(ptr_destino, datos.st_size);

	//Cerramos los descriptores de fichero
	close(fd_origen);
	close(fd_destino);
		 

	
	return 0;
}

