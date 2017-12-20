#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
/*0 - stdin
1 - stdout
2 - stderr*/
 /*STDIN_FILENO  

    Standard input value stdin. Its value is 0.
STDOUT_FILENO  

    Standard output value stdout. Its value is 1.
STDERR_FILENO  */
//fd[0] is set up for reading, fd[1] is set up for writing

void errorAperturaFichero(){
	printf("\nNo se consiguio abrir el fichero pasado por parametro\n");
	exit(-1);
}


int main (int argc, char *argv[])
{
	int fd[2],estado;
	pid_t pid;
	if(argc != 4){
		printf("\nFaltan argumentos\n");
		exit(-1);
	}
	if (strcmp(argv[2], "|") == 0 ){
		pipe(fd);
		if ((pid = fork()) < 0){
			perror("\nError en el fork");
			exit(-1);
		}
		if(pid == 0){
			//perror("\nError en el fcnlt 1");
			close(fd[0]);
			close(1);
			
			if(fcntl(fd[1], F_DUPFD, 1) == -1) {
				perror("\nError en el fcnlt 1");
				exit(-1);
			}

			execlp(argv[1],argv[1],NULL);
			
			exit(0);
		}else{
			
			close(fd[1]);
			close(0);
			if(fcntl(fd[0], F_DUPFD, 0) == -1) {
				perror("\nError en el fcnlt 2");
				exit(-1);
			}
			execlp(argv[3],argv[3],NULL);
			
			exit(0);
		}
		
	}else{
		printf("\nEl segundo parametro tiene que ser < o >\n");
			exit(-1);
	}
	

	

   return 0;
}
