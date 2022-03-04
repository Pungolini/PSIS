/* Change the previous exercise so that each thread, when terminating will return the
character count to the main thread.
The main thread will receive this value and print it in the screen.
Experiment the following ways to get a value out of the thread:
• pthread_exit(count)
• pthread_exit(&count)
• pthread_exit(ptr)
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread_func( void* arg)
{
	int cnt;
	while ( getchar() != '\n')
		++cnt;

	printf("Read %d chars\n",cnt );
	pthread_join(*(pthread_t*)arg,NULL);
}



int main(int argc, char const *argv[])
{
	int n_threads;
	pthread_t *tid;


	// verify command line arguments
	if(argc != 2 || sscanf(argv[1], "%d", & n_threads) != 1)
		exit(-1);

	// Array of thread identifiers
	if((tid = (pthread_t*)malloc(n_threads*(sizeof *tid))) == NULL) EXIT_FAILURE;

	// Create threads
	printf("Creating %d threads...\n",n_threads );
	for (int i = 0; i < n_threads; ++i)
	{
		pthread_create(&tid[i], NULL, thread_func, NULL);
	}
	getchar();
	return 0;
}