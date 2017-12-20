#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
/*LOCK_SH Coloque un bloqueo compartido. Más de un proceso puede contener una
bloqueo compartido para un archivo dado en un momento dado.

LOCK_EX Coloca un candado exclusivo. Solo un proceso puede contener una
bloqueo exclusivo para un archivo dado en un momento dado.

LOCK_UN Eliminar un bloqueo existente en este proceso.*/


int main (int argc, char *argv[])
{
	int fd;
	
	if((fd = open(argv[1],O_RDWR )) < 0){
		perror("\nError al abrir el fichero");
		exit(-1);
	}
	if(flock(fd, LOCK_EX) == -1){
		perror("\nError al bloquear fichero");
		exit(-1);
	}
	sleep(10);
	close(fd);
	
	printf("\n Se libero el fichero\n");
	

   return 0;
}

