/*
tarea1.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
Probar tras la ejecución del programa: $>cat archivo y $> od -c archivo
*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	/* Primitive system data types for abstraction\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
			*/
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";

int main(int argc, char *argv[])
{
int fd;
//se guarda el descripotro de abrir el fichero en fd
//Si diera error entraria en el if al devolver -1 sino devuelve el numero de bite escritos
//O_CREAT = Si no exixte se crea 
//O_TRUNC = Se trunca a 0 (fd = 0) y se empieza a escribir desde el principo al abrirse con o_wronly
//O_WRONLY = Solo escritura
//S_IRUSR = Usuario tiene permisos de lectura (Depende del umask mode  & ~umask)
//S_IWUSR = Usuario tiene permisos de escritura (Depende del umask mode  & ~umask)

if( (fd=open("archivo",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
	printf("\nError %d en open",errno);
	perror("\nError en open");
	exit(EXIT_FAILURE);
}
//Se escribe el buffer primero si no se escribe correctamente devolvera un 
//descriptor distinto de 10 puesto que al abrirlo nos devolvio el 0
//fd = descriptor 
//buf1 = datos a leer para escribir
//10 = numero de datos que se escribiran
if(write(fd,buf1,10) != 10) {
	perror("\nError en primer write");
	exit(EXIT_FAILURE);
}
//Se mueve el descriptor hasta el final y si da error devuelve -1
//fd = descriptor
//SEEK_SET = Si se pasa se queda en la ultima posicion
// 40 = La posicion del puntero fd En este caso se lleva la final ya que hay menos que 40
if(lseek(fd,40,SEEK_SET) < 0) {
	perror("\nError en lseek");
	exit(EXIT_FAILURE);
}

if(write(fd,buf2,10) != 10) {
	perror("\nError en segundo write");
	exit(EXIT_FAILURE);
}

return EXIT_SUCCESS;
}
