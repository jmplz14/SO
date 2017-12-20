#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {

	//Declaracion de variables
	struct flock cerrojo;
	int fd, i;

	if(argc != 2) {

		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <archivo>\n\n", argv[0]);
		exit(1);

	} else {

		//Extraemos el nombre del archivo a usar (por comodidad)
		char *str_file = argv[1];

		//Abrimos el archivo
		if ((fd=open(str_file, O_RDWR|O_CREAT, 0662)) == -1 ){
			perror("Fallo al abrir el archivo");
			return 0;
		}

		cerrojo.l_type=F_WRLCK;
		cerrojo.l_whence=SEEK_SET;
		cerrojo.l_start=0;
		cerrojo.l_len=0; //Bloquearemos el archivo entero

		//Intentamos un bloqueo de escritura del archivo
		//printf ("Intentando bloquear %s\n", str_file);
		if (fcntl (fd, F_SETLK, &cerrojo) == -1) {
		
			//Si el cerrojo falla, pintamos un mensaje
			printf("El programa ya esta en ejecucion\n");
			exit(-1);
		}else{
			struct stat datos;
			if(stat(argv[1], &datos))
				printf("%s", strerror(errno));
			 else if(datos.st_size <= 1){
				printf("bucle\n");
				while(1);
			 }else{
				 printf("El fichero no esta vacio\n");
				exit(-1);
			 }
			
			
		} 

		return 0;
	}
}
