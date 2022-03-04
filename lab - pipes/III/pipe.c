#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>


#define n_threads 4


int globalArrRes[4] = {0};
int globalarraypipes[4][2];
int globalArrayPrim[4] = {2,3,5,7};



void *thread_func(void *arg)
{
	int t_index = (intptr_t)arg;
	int num_to_read;

	// Ler da pipe
	
	while(read(globalarraypipes[t_index][0], &num_to_read, sizeof(int)) > 0)
	{
		printf("read %d tindex %d\n",num_to_read, t_index);
	
		// Atualizar array de resultados
		if ((num_to_read % globalArrayPrim[t_index]) == 0)
			globalArrRes[t_index]++;
	}

}

int main(int argc, char const *argv[])
{
	int num = 0;
	int pipe_index;

	pthread_t tid[n_threads];

	// Create pipes
	for (int i = 0; i < n_threads; ++i)
	{
		if (pipe(globalarraypipes[i]) == -1)
			exit(EXIT_FAILURE);
	}


	// Create threads
	for (int i = 0; i < n_threads; ++i)
	{
		pthread_create(&tid[i], NULL, thread_func,(void*)(intptr_t)i);
	}


	// Read from the keyboard
	do
	{
		printf("\n%s","num: " );
		scanf("%d",&num);


		write(globalarraypipes[0][1],&num, sizeof(int));			
		write(globalarraypipes[1][1],&num, sizeof(int));
		write(globalarraypipes[2][1],&num, sizeof(int));
		write(globalarraypipes[3][1],&num, sizeof(int));

		
	} while (num >= 0);

	getchar();

	// Print results
	for (int i = 0; i < 4; ++i)
		printf("%d\t",globalArrRes[i] );
	putchar('\n');

	return 0;
}