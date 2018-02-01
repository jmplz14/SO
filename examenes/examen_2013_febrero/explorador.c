#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

//solo realizado el primer ejercicio
//No pongo comprobaciones por rapidez
int main (int argc, char *argv[]){
	
	char datos[255],id_inicial[12];
	struct stat atributos;
	lstat(argv[1],&atributos);
	int propietario_inicial = atributos.st_uid, id_actual,fd_tuberia;
	
	fd_tuberia=open("tuberia",O_WRONLY);
	
	while(1){
		
		lstat(argv[1],&atributos);
		id_actual = atributos.st_uid; 
		
		if(propietario_inicial != id_actual ){
			strcpy(datos,argv[1]);
			sprintf(id_inicial,"%d", id_actual);
			strcat(datos," ");
			strcat(datos,id_inicial);
			write(fd_tuberia,datos,strlen(datos));
			propietario_inicial = id_actual;
		}
		
	}
	
	
	
}
