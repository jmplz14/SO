#define _XOPEN_SOURCE 500 
#include <ftw.h>
#include <stdio.h>
int num_ficheros = 0;
long ocupado = 0;
int nftwfunc(const char *filename, const struct stat *statptr, int fileflags, struct FTW *pfwt){
    //char cadena[500];
    //sprintf(cadena,"%s",filename);
    if ( fileflags  == FTW_NS ){
		printf("\nError al intentar acceder a los atributos dearchivo de %s\n", filename); 
		perror("\nError en lstat");
		
	}else
		if(fileflags == FTW_F){
			if((statptr->st_mode & (S_IXGRP|S_IXOTH)) == (S_IXGRP|S_IXOTH)){
				printf("%s %ld\n",filename, statptr->st_ino);
				num_ficheros += 1;
				ocupado += statptr->st_size;
			}
		}
    return 0;
}
int main(int argc, char *argv[]){
	char *startpath = "/tmp";
	int fd_limit = 5;
	int flags = FTW_PHYS;
	int ret;


	ret = nftw((argc < 2) ? "." : argv[1], nftwfunc, fd_limit, flags);
	printf("Existen %d archivos con permosos x para grupo y otros\n",num_ficheros);
	printf("El tamaño ocupado por estos ficheros es de  %ld bytes\n",ocupado);
}
