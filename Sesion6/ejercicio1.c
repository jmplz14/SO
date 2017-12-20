#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int abrirLectura(const char *documento){
	int fd;
	fd = open(documento,  O_RDONLY);
	return fd;
}

int abrirEscritura(char *documento){
	int fd;
	fd = open(documento, O_WRONLY|O_CREAT|O_TRUNC, 0772);
	return fd;
}
void errorAperturaFichero(){
	printf("\nNo se consiguio abrir el fichero pasado por parametro\n");
	exit(-1);
}


int main (int argc, char *argv[])
{
	int fd;
	if(argc != 4){
		printf("\nFaltan argumentos\n");
		exit(-1);
	}
	if (strcmp(argv[2], ">") == 0 ){
		if((fd=abrirEscritura(argv[3])) < 0){
			errorAperturaFichero();
		}
		close(1);
		if(fcntl(fd,F_DUPFD,1) == -1){
			printf("\nFallo al duplicar el fd\n");
			exit(-1);
		}
		
	}else if(strcmp(argv[2], "<") == 0){
		
		if((fd=abrirLectura(argv[3])) < 0){
			errorAperturaFichero();
		}
		close(0);
		if(fcntl(fd,F_DUPFD,0) == -1){
			printf("\nFallo al duplicar el fd\n");
			exit(-1);
		}
	}else{
		printf("\nEl segundo parametro tiene que ser < o >\n");
			exit(-1);
	}
	
	execlp(argv[1],argv[1],NULL);

	

   return 0;
}
