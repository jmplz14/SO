#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	const int nprocs = 20;
	pid_t hijo;
	int i;
	for( i = 0; i < nprocs; i++){
		if((hijo = fork()) == -1){
			printf("Error al crera un hijo");
			exit(EXIT_FAILURE);
		}
		printf("\n PID: %d PIDPADRE: %d \n" , getpid(),getppid());
		if(hijo)
			break;
		
	}
		
	return EXIT_SUCCESS;
}
