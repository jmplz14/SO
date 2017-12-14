#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	pid_t hijo;
	int estado;
	if(argc == 1){
			perror("\nError en el numero de argumentos\n");
			exit(EXIT_FAILURE);
	}
	if((hijo = fork()) == -1){
		printf("Error al crera un hijo");
		exit(EXIT_FAILURE);
	}
	
	if(hijo == 0){
		if( (execl(argv[1], argv[2], NULL)) < 0){
			printf("Error al hacer excl");
			exit(EXIT_FAILURE);
		}
		
	}
	
	wait(&estado);
	printf("\nmi hijo termino\n");
	return EXIT_SUCCESS;
}
