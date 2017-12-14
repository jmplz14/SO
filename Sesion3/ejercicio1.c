#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
		
		const int base = 10, num_padre = 4, num_hijo = 2 ;
		int numero;
		char *final;
		if(argc != 2){
			perror("\nError en el numero de argumentos\n");
			exit(EXIT_FAILURE);
		}
		numero = strtol(argv[1],&final,base);
		if(argv[1] != final){
			pid_t hijo = fork();
			if (hijo == 0){
				if((numero % num_hijo) == 0)
					printf("\nEl numero es par\n");
				else
					printf("\nEl numero es inpar\n");
			}else{
				if((numero % num_padre) == 0)
					printf("\nEl numero es divisible por %d \n", num_padre);
				else
					printf("\nEl numero no es divisible por %d \n", num_padre);
			}
		}else{
			perror("\nNo se introdujo un entero valido\n");
			exit(EXIT_FAILURE);
		}
	return EXIT_SUCCESS;
}
