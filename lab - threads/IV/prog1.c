#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Function to be sent to all threads
// Writes all prime numbers
void *thread_func(void* _loopVar)
{
	int *count = (int*)_loopVar;


	for (int i = 0; i < *count; ++i)
		printf("Printing %d times from thread %lx\t%d\n",*count,pthread_self(),i );
	putchar('\n');

	return NULL;
}


int main(int argc, char const *argv[])
{
	int n_threads,n;
	pthread_t *tid;
	srand(time(NULL));


	// verify command line arguments
	if(argc != 2 || sscanf(argv[1], "%d", & n_threads) != 1)
		exit(-1);

	// Array of thread identifiers
	if((tid = (pthread_t*)malloc(n_threads*(sizeof *tid))) == NULL) EXIT_FAILURE;

	// Create threads
	printf("Creating %d threads...\n",n_threads );
	for (int i = 0; i < n_threads; ++i)
	{
		n = random()%21;
		sleep(1);
		pthread_create(&tid[i], NULL, thread_func, &n);
	}

	getchar();
	return EXIT_SUCCESS;
}