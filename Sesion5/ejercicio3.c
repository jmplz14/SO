#include <stdio.h>
#include <signal.h>

int main(){
	
	sigset_t mascara;
	//SE INICIAN LAS MASCARAS A 0
	sigemptyset(&mascara);
	//Solo la saca de parada la señal sisgusr1
	sigaddset(&mascara, ~SIGUSR1);
	
	sigsuspend(&mascara);
}
