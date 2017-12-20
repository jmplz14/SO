#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char * argv[]) 
{

	struct flock cerrojo;
	int fd;
	while (--argc > 0 ) {
		if ((fd=open(*++argv, O_RDWR)) == -1 ){
			perror("open fallo");
			continue;
		}
	//tipo de bloqueo de escritura solo
	cerrojo.l_type=F_WRLCK;
	//posicion desde donde se empieza el desplazamiento del cerrojo
	cerrojo.l_whence=SEEK_SET;
	//desplazamiento a realizar desde whence
	cerrojo.l_start=0;
	//Numero de bytes que se bloquena desde whence+start
	cerrojo.l_len=0;
	
	/* intentamos un bloqueo de escritura del archivo completo */

	while (fcntl (fd, F_SETLK, &cerrojo) == -1) {
		/* si el cerrojo falla, vemos quien lo bloquea */
		while (fcntl (fd, F_SETLK, &cerrojo) == -1 && cerrojo.l_type != F_UNLCK) {
			printf ("%s bloqueado por %d desde %lld hasta %lld para %c", *argv, cerrojo.l_pid, cerrojo.l_start, cerrojo.l_len, cerrojo.l_type==F_WRLCK ? 'w':'r');
		//Si el numero de bytes del cerrojo es distinto de 0(Se bloquearia todo el archivo desde whence + start) saldria porque no puede reservar nada
		if (!cerrojo.l_len) break;
		//Si no se en estar se sumaria el el numero de bytes del cerrojo para reservar apartir de aqui todo lo restante
		cerrojo.l_start += cerrojo.l_len;
		cerrojo.l_len = 0;
		} /*mientras existan cerrojos de otros procesos */
	} /*mientras el bloqueo no tenga exito */

	/* ahora el bloqueo tiene exito y podemos procesar el archivo*/
	printf ("procesando el archivo\n");
	/* una vez finalizado el trabajo, desbloqueamos el archivo entero*/
	cerrojo.l_type=F_UNLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	if (fcntl (fd, F_SETLKW, &cerrojo) == -1) perror ("Desbloqueo");
	}
	return 0;
}
