#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//solo realizado el primer ejercicio
//No pongo comprobaciones por rapidez
int main (int argc, char *argv[]){
	
	const char *tuberia="tuberia";

	mkfifo(tuberia,0660);
	int pid_1,pid_2,fd_tuberia,leidos;
	char buffer[100];
	pid_1 = fork();
	if(pid_1 == 0){
		execlp("./explorador","explorador",(char *)argv[1],(char *)NULL);
		exit(0);
	}
	pid_2 = fork();
	if(pid_2 == 0){
		execlp("./explorador","explorador",(char *)argv[2],(char *)NULL);
		exit(0);
	}
	fd_tuberia=open(tuberia,O_RDONLY);
	
	while(1){
		leidos=read(fd_tuberia,buffer,100);
		printf("%s \n",buffer);
	}
	
	
	
}
