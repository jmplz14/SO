//productorFIFO.c
//Productor que usa mecanismo de comunicacion FIFO

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#define ARCHIVO_FIFO "ComunicacionFIFO"
int esPrimo(int n){
	int primo = 1, i;
	for(i=2; i<=n/2 && primo == 1 ; ++i){
		
        if(n%i==0){
			
            primo = 0;
        }
        
    }	
    return primo;
}

int main(int argc, char *argv[])
{
	char str[12];
	int cauce_menor[2], cauce_mayor[2],leidos, primo;
	pid_t hijo;
	int inicio, centro, fin;
	//Comprobar el uso correcto del programa
	if(argc != 3) {
	printf("\npSe necesitan dos argumentos faltan argumentos (mensaje)");
	exit(EXIT_FAILURE);
	}

	inicio = strtol(argv[1],NULL,10);
	fin = strtol(argv[2],NULL,10);
	char buffer[80];
	char *bandera = "fin"; 
	centro = inicio + ( (fin - inicio) / 2 );


	pipe(cauce_menor);
	pipe(cauce_mayor);

	if ( (hijo= fork())<0) {
		perror("\Error en fork");
		exit(EXIT_FAILURE);
	}

	if (hijo == 0) { // ls

		close(cauce_menor[0]);
		int i;
		
		for(i = inicio; i < centro; i++){
			
			if(esPrimo(i) == 1){
				write(cauce_menor[1], &i, sizeof(int));
				
			}
		}	
		exit(0);
	}
	else { 
		close(cauce_menor[1]);
	}

	if ( (hijo= fork())<0) {
		perror("\Error en fork");
		exit(EXIT_FAILURE);
	}


	if (hijo == 0) {
		
		close(cauce_mayor[0]);
		//dup2(cauce_menor[1],STDOUT_FILENO);
		int i;
		for(i = centro; i <= fin; i++)
			if(esPrimo(i) == 1)
				write(cauce_mayor[1], &i, sizeof(int));
		exit(0);
	}
	else { 
		close(cauce_mayor[1]);
	}
		printf("Numero primer exclavo: ");
		while((leidos = read(cauce_menor[0],&primo, sizeof(int))) > 0){
			printf("%d ", primo);
		}
		close(cauce_menor[0]);
		printf("\nNumero Segundo exclavo: ");
		
		while((leidos = read(cauce_mayor[0],&primo, sizeof(int))) > 0){
			printf("%d ", primo);
		}
		close(cauce_mayor[0]);
		printf("\n");
		

	return EXIT_SUCCESS;
}

