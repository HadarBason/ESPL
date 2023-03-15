#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void signal_handler(int signo);

int main(int argc, char **argv){ 
  	if (signal(SIGINT, signal_handler) == SIG_ERR)
  		printf("\ncan not catch SIGINT\n");
  	if (signal(SIGCONT, signal_handler) == SIG_ERR)
  		printf("\ncan not catch SIGCONT\n");
  	if (signal(SIGTSTP, signal_handler) == SIG_ERR)
  		printf("\ncan not catch SIGTSTP\n");	

	while(1) 
		sleep(2);
	
	return 0;
}


void signal_handler(int signo)
{
	char* name = strsignal(signo);
	printf("\nLooper handling %s\n",name);
  	if (signo == SIGCONT)
		signal(SIGTSTP, signal_handler);	
  	if (signo == SIGTSTP)
		signal(SIGCONT, signal_handler);	  
	signal(signo,SIG_DFL);
	raise(signo);	  	  
}
