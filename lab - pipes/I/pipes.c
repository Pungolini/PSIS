#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX		99999 + 1


int isPrime(int n)
{
	int i, flag = 0;
	
	for(i = 2; i <= n/2; ++i)
    {
        // condition for nonprime number
        if(n%i == 0)
        {
            flag = 1;
            break;
        }
    }

    if (n == 1) 
    	return 0;
    else 
    {
        if (flag == 0)
          return 1;
        else
          return 0;
     }
    
    return 0;	
}


int main(int argc, char const *argv[])
{
	int n_slaves, n_rand,n_prime = 0;
	int slave_id, fd[2], number_to_read;
	srandom(getpid());

	// verify command line arguments
	sscanf(argv[1], "%d", & n_slaves);
	sscanf(argv[2], "%d", & n_rand);


	// Create a pipe
	if(pipe(fd) == -1)
	{
		puts("Error creating pipe");
		exit(EXIT_FAILURE);
	}



	for (int i = 0; i < n_slaves; ++i)
	{
		// Child process
		if ((slave_id = fork()) == 0)
		{
			

			// ?????????????
			close(fd[1]);


			// Read from pipe
			while(read(fd[0],&number_to_read, sizeof(number_to_read)) > 0)
			{
				if (isPrime(number_to_read))
				{
					printf("n_rand  = %d isPrime = %d\n",n_rand,isPrime(number_to_read));
					++n_prime;
				}
			}
			printf("Process id %d\tNumber of primes %d\n",slave_id,n_prime );
			exit(EXIT_SUCCESS);
		}

	}


	for (int i = 0; i < n_rand; ++i)
	{
		n_rand = random()% MAX;

		// Write to pipe
		write(fd[1],&n_rand, sizeof(n_rand));
	}
	close(fd[1]);





	return 0;
}