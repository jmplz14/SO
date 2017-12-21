#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main (int argc, char *argv[])
{
	char buffer[1024];
	char nombre_fifo_proxy[1024];
	struct flock cerrojo;
	int num_datos_leidos,descriptor_bloqueo;
	FILE *fichero_temporal = tmpfile();
	//Leemos los datos del cliente y se escribe en el temporal
	while(num_datos_leidos=read(STDIN_FILENO, buffer, 1024) > 0)
		fwrite(buffer,sizeof(char),num_datos_leidos,fichero_temporal);
		
	//abrimos el fichero de bloqueo
	descriptor_bloqueo = open("bloqueo", O_WRONLY|O_CREAT);
	
	//se configura para bloquear todo el fichero
	cerrojo.l_type=F_WRLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	
	//Se realiza el bloqueo
	fcntl (descriptor_bloqueo, F_SETLK, &cerrojo);

	while(num_datos_leidos=fread( buffer, sizeof(char),1024,fichero_temporal) > 0)
		write(STDOUT_FILENO,buffer,num_datos_leidos);
		
	cerrojo.l_type=F_UNLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
		
	fcntl (descriptor_bloqueo, F_SETLK, &cerrojo);
	sprintf(nombre_fifo_proxy,"fifo.%d", getpid());
	unlink(nombre_fifo_proxy);
	exit(0);
}
