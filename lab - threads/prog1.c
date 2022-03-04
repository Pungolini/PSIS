#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>


int countChar(int *cnt)
{
	while(getchar() != '\n')
		(*cnt)++;

	return 0;
}

void createThreads(int n_threads, pthread_t tid, void* (*thread_func)(void * arg))
{
	printf("Creating %d Threads...\n", n_threads);

	for (int n = 0; n < n_threads; n++)
		pthread_create(&tid, NULL, thread_func, &tid);
}

void *thread_func (void * arg, char ch){
  int n = 0,cnt = 0;
  // Terminate thread
  if (!countChar(&cnt))
  {
  	printf("char read in thread %lx: %d\n",pthread_self(),cnt );
  	pthread_join(*(pthread_t*)arg,NULL);
  }


  printf("exiting %lx \n", pthread_self() );
}


int main(int argc, char * argv[])
{
	int n_threads,n, n_char;
	pthread_t thread_id;
	char ch;

	// verify command line arguments
	if(argc == 1 || sscanf(argv[1], "%d", & n_threads) != 1)
		exit(-1);

	// Create threads
	createThreads(n_threads, thread_id, thread_func(&thread_id,ch));

	if(countChar(&n_char) == 0)
	{
		printf("char read main: %d\n",n_char );
  		//pthread_exit(NULL);
  		pthread_join(pthread_self(),NULL);
	}

	exit(0);


}
