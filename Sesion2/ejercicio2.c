#include<dirent.h>
#include<sys/types.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){
	int permisos_introducidos;
	int permisos_actuales, permisos_cambiados;
	DIR *directorio;
	char *endptr, *str;
	struct dirent *datos;
	struct stat metadatos; 
	if(argc != 3) {
		printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
		exit(EXIT_FAILURE);
	}
	
	if((directorio = opendir(argv[1])) == NULL ){
		printf("\nError al intentar abir el directorio %s",argv[1]);
		perror("\nError en opendir\n");
		exit(EXIT_FAILURE);
	}
	chdir(argv[1]);
	permisos_introducidos = strtol(argv[2],&endptr,8);

	while ((datos=readdir(directorio)) != NULL) {
        
        if ( strcmp(datos->d_name, ".") != 0 && strcmp(datos->d_name, "..") != 0 )
        {
            
             
			if(stat(datos->d_name,&metadatos) < 0){
					printf("\nError al intentar acceder a los atributos dearchivo de %s\n", datos->d_name); 
					perror("\nError en lstat");
			}else{
				permisos_actuales = metadatos.st_mode & (S_IRWXU|S_IRWXG|S_IRWXO);
				
				if(chmod(datos->d_name,permisos_introducidos) < 0) {
					printf("No se cambieron los permisos de %s: codigo errno = %d %o \n", datos->d_name, errno,permisos_actuales);
					//perror("\nError en chmod para archivo2");
					
					
					
				}else{
					printf("%s: %o %o \n", datos->d_name, permisos_actuales, permisos_introducidos );
				}
			}
			
        } 
    }

return EXIT_SUCCESS;
}

