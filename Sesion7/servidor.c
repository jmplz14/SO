#include <sys/file.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// cod 1 de errores fallo al ejecutar servidor por parametros
// cod 2 un archivo no se pudeo abrir
// cod 3 fallo al bloquear un archivo
// cod 4 error al establecer una señal
// cod 5 error al crear archivo fifo
const char bloqueo_servidor[] = "bloqueoSerividor";
const int tamano_nombres = 50, operaciones_ficheros = 1024;
//Funcion que espera la finalizacion de todos los hijos
static void mimanejador(int sigNum){
  while(wait(NULL) > 0);
  
}

int main(int argc, char *argv[]){
	int descriptor_bloqueo, descriptor_fifo_lectura, descriptor_fifo_escritura,descriptor_bloqueo_proxys;
	int leido_clientes, estado_leido_cliente;
	pid_t pid_hijo;
	struct flock fichero_bloqueo;
	struct sigaction senales_hijos;
	char nombre_fifo_escritura[tamano_nombres];
	char nombre_fifo_lectura[tamano_nombres], nombre_fifo_cliente[tamano_nombres];
	
	
	if(argc != 2){	
		perror("\nError en el numero de parametros del servidor\n");
		exit(1);
	}

	//abrimos el fichero de bloqueo;
	if((descriptor_bloqueo = open(bloqueo_servidor, O_RDWR|O_CREAT, 0662)) < 0 ){
		perror("\nError al abrir el fichero de bloqueo \n");
		exit(2);
	}
	//Se bloquea el fichero de bloqueo
	if(flock(descriptor_bloqueo, LOCK_EX|LOCK_NB) == -1){
		perror("\nYa hay una estancia de servidor ejecutandose");
		exit(3);
	}
	//Activamos la funcion que espera por los hijos
	senales_hijos.sa_handler = mimanejador; 
	sigemptyset(&senales_hijos.sa_mask);
	//Reiniciar las funciones que hayan sido interrumpidas por un manejador
	senales_hijos.sa_flags = 0;
	
	if(sigaction(SIGCHLD,&senales_hijos, NULL) == -1){
		perror("\nError al activar mi manejador para que que espere a los hijos");
		exit(4);
	}
	
	umask(0);
	//Se añaden las extensiones a los fifos
	strcpy(nombre_fifo_escritura,argv[1]);
	strcat(nombre_fifo_escritura,"e");
	strcpy(nombre_fifo_lectura,argv[1]);
	strcat(nombre_fifo_lectura,"s");
	
	//Se crean los ficheros fifo
	if(mknod(nombre_fifo_lectura, S_IRWXU, 0) == -1){
		perror("\nError al crear fifio de lectura");
		exit(5);
	}
	
	if(mknod(nombre_fifo_escritura, S_IRWXU, 0)== -1){
		perror("\nError al crear fifio de escritura");
		exit(5);
	}
	//abrir los fichero fifo
	if((descriptor_fifo_lectura = open(nombre_fifo_lectura, O_RDWR)) == -1){
		perror("\nError al abrir fichero de lectura");
		exit(2);
	}
	if((descriptor_fifo_escritura = open(nombre_fifo_escritura, O_RDWR)) == -1){
		perror("\nError al abrir fichero de escritura");
		exit(2);
	}
	
	//Creamos el fichero bloque0 para los proxys
	if((descriptor_bloqueo_proxys = open("bloqueo",O_RDWR|O_CREAT, 0662)) == -1){
		perror("\nError al abrir fichero de bloqueo para proxys");
		exit(2);
	}
	/*leido_clientes=read(descriptor_fifo_escritura, &estado_leido_cliente, sizeof(int));
	printf("leidooo %d", leido_clientes);*/
	while(1){
		if((leido_clientes=read(descriptor_fifo_escritura, &estado_leido_cliente, sizeof(int))) != 0){
			
			
			
			pid_hijo = fork();
			
			
			if(pid_hijo == 0){
				pid_t pid = getpid();
				int descriptor_fifo_proxy;
				sprintf(nombre_fifo_cliente,"%d", pid);
				mkfifo(nombre_fifo_cliente, S_IRWXU);
				write(descriptor_fifo_lectura,nombre_fifo_cliente, sizeof(int));
				descriptor_fifo_proxy = open(nombre_fifo_cliente, O_RDWR);
				close(0);
				fcntl(descriptor_fifo_proxy, F_DUPFD, 0);
				execlp("./proxy","proxy",NULL);
			}
			
		}
	}
	
	//Con esto se libera el fichero que permite a otro servidor ejecutarse si no hay mas de 1 a la vez
	close(descriptor_bloqueo);
	
	return 0;
}
