//fork
#include <unistd.h>
//printf
#include <stdio.h>
//getpid
#include <sys/types.h>
#include <unistd.h>
//sprintf
#include <stdio.h>
//exit
#include <stdlib.h>



int main (int argc, char *argv[]){
	int i, pid,estado,fd_pipe[2], leidos, num_total_bloques = 0, num_bloques;
	pid_t wpid;
	pipe(fd_pipe);
	for(i = 1; i < argc; i++){
		if((pid = fork()) < 0){
			perror("La creacion del hijo fallo\n");
			exit(-1);
		}else if(pid == 0){
			sleep(1);
			close(fd_pipe[0]);
			close(1);
			dup(fd_pipe[1]);
			char ficheroSalida[1024];
			int pid = getpid();
			sprintf(ficheroSalida,"fichero.%d", pid);
			execlp("./catdir","catdir",(char *)argv[i],ficheroSalida,(char *)NULL);
			
			exit(0);
		}
	}
	close(fd_pipe[1]);
	//while((wpid = wait(&estado)) > 0);
	for(i = 1; i < argc; i++){
		//printf("sale el hijo %d", i );
		wait(&estado);
	}
	while( (leidos = read(fd_pipe[0],&num_bloques, sizeof(num_bloques))) != 0){
			num_total_bloques += num_bloques;
			//printf("Total de bloques = %d", num_bloques);

	}
	printf("Total de bloques = %d \n", num_total_bloques );
}
