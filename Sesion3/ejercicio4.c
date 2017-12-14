#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
/*		<-1, es decir, espera por cualquier proceso hijo cuyo ID de grupo de proceso sea
              igual al valor absoluto de pid.

       -1 significa esperar por cualquier proceso hijo.

       0 significa esperar por cualquier proceso hijo cuyo ID de grupo de proceso es
              igual a la del proceso de llamada.

       > 0 significa esperar al niño cuyo ID de proceso es igual a
              valor de pid*/
int main(int argc, char *argv[]){
	int nprocs = 5;
	pid_t pids[5];
	pid_t hijo;
	int i,estado;
	if(setvbuf(stdout,NULL,_IONBF,0)) {
		perror("\nError en setvbuf") ;
	}
	for( i = 0; i < nprocs; i++){
		if((hijo = fork()) == -1){
			printf("Error al crera un hijo");
			exit(EXIT_FAILURE);
		}
		
		if(hijo == 0){
			printf("Soy el hijo %d numero %d\n" , getpid(), i + 1);
			exit(EXIT_SUCCESS);
		}else{
			pids[i] = hijo;
		}
		
	}
	int num_hijos = nprocs;
	for ( i = 0; i < nprocs ; i += 2){
		waitpid(pids[i],&estado);
		printf("Ha terminado mi hijo con pid %d\n", pids[i]);
		printf(" Me quedan  %i hijos \n" , num_hijos--);
	}
	for ( i = 1; i < nprocs ; i += 2){
		
		waitpid(pids[i],&estado);
		printf("Ha terminado mi hijo con pid %d\n" , pids[i]);
		printf(" Me quedan  %i hijos \n" , num_hijos--);
	}
	
	return EXIT_SUCCESS;
}
