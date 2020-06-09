#include<signal.h>
#include<stdio.h>
#include<unistd.h>



void ouch(int sig)
{
	printf("OUCH! - I got signal %d\n",sig);
}

int main()
{
	struct sigaction act;
	act.sa_handler = ouch;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT,&act,0);
	
	while(1) {
		printf("Hello World!\n");
		sleep(1);
	}
/* how it works
 * The program calls sigaction instead
 * of signal to set the signal handler for
 * Ctrl+C(SIGINT) to the function ouch.
 * It first has to setup a sigaction 
 * structure that contains the handler,  a signal mask,
 * and flags. In this case, you don't need 
 * any flags, and an empty mask is created with the new function,igemptyset.
 }



