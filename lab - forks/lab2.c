#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


void show_message(int n)
{
	int _delay = random()%11;
	//alarm(_delay);
	printf("%d\n",n );
	
	//signal(SIGALRM,show_message);
}

void handle()
{

}

int main(int argc, char const *argv[])
{

	//alarm(1);
	signal(SIGALRM,handle);

	for (int n = 0;;++n)
	{
		sleep(1);
		printf("\t\t\t\t%d\n", n);
		if (n == 50)
		{
			n = 0;
		}


	}


	//counter();
	return 0;
}

/*
int main(int argc, char const *argv[])
{
	int _delay = 0;
	for (int i = 0; i < 10; ++i)
	{
		_delay = random()%11;

		 if (!fork())
		{
			
			exit(EXIT_SUCCESS);
			
		}

	for (int i = 0; i < 10; ++i)
	{
		printf("son: %d\tparent: %d\tSleep: %d\n",getpid(),getppid(),(int)_delay);
		wait(&_delay);
		printf("Child %d terminated \nSleep: %d\n",_delay );
	}
}
	
	return 0;
}*/
