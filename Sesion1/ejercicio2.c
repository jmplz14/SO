#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>



int main(int argc, char *argv[]){
int fd, cont = 0, creado, copiado,lectura;
	ssize_t terminado;
	char leido[80];
	char bloque[100], cabecera[100];
	char salto[2] = "\n";
	int tamano_fichero = 0;
	if(argc == 1){
		fd = STDIN_FILENO;
	}else{
		//Se abre el fichero
		if((fd = open(argv[1],O_RDONLY)) < 0){
			printf("\nError %d en open",errno);
			perror("\nError en open");
			exit(EXIT_FAILURE);
		}
	}
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if ((creado = open("secundario.txt", O_RDWR | O_CREAT, S_IRUSR|S_IWUSR)) < 0){
		printf("\nError %d en creat",errno);
		perror("\nError en creat");
		exit(EXIT_FAILURE);
	}
	
	 while((lectura = read (fd, &leido, 80)) != 0){
		cont++;
		sprintf(bloque, "Bloque %d\n", cont);
		write (creado, bloque, strlen(bloque));
		write (creado, salto, strlen(salto));
        write (creado, &leido, lectura);
            
    }
    close(fd);
 
	
	sprintf(cabecera, "El nº bloques es %d\n", cont);
	if ((copiado = open("salida.txt", O_WRONLY | O_CREAT, S_IRUSR|S_IWUSR)) < 0){
		printf("\nError %d en creat",errno);
		perror("\nError en creat");
		exit(EXIT_FAILURE);
	}
	write (copiado, cabecera, strlen(cabecera));
	lseek(creado,0,SEEK_SET);
	while((lectura = read (creado, &leido, 80)) != 0){
        write (copiado, &leido, lectura);       
    }
	close(copiado);
    close(creado);
    remove("secundario.txt");
	




return EXIT_SUCCESS;
}
