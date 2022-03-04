#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "sock_dg.h"


#define RANGE 101


int n;


void *send_thread_func( void * arg)
{
	int sock_fd = *(int*)arg;
	int nbytes = recv(sock_fd, &n, sizeof(n), 0);
	printf("Prime number %d received\tnbytes %d\n", n, nbytes);
}



int main()
{
	struct sockaddr_un server_addr;
	struct sockaddr_un local_addr;
	int nbytes,error,td_err,rand_num,sock_fd;
	char linha[1000],path[200] = "/tmp/", pid[100];

	pthread_t tid;


	/* Start random seed generator */
	srandom(getpid());



	if ((sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1);
	}

	local_addr.sun_family = AF_UNIX;

	// path = "/tmp/" + getpid
	sprintf(pid,"%d",getpid());
	strcat(path,pid);
	printf("path = %s\n",path);

	strcpy(local_addr.sun_path, path);


	// Thread
	if(td_err = pthread_create(&tid, NULL,send_thread_func, (void*)&sock_fd) != 0)
	{
		puts("Error creating Thread...");
		exit(td_err);
	} 


	// exercise 2: comment the following line
 	error = bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));

	if(error == -1)
	{
		perror("bind");
		exit(-1);
	}



	printf(" Socket created \n Ready to send\n");

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_ADDRESS);
	while(1)
	{
		// Gerar numero aleatorio
		rand_num = random()%RANGE;

		// Mandar para o servidor
		nbytes = sendto(sock_fd, &n, sizeof(n), 0,(const struct sockaddr *) &server_addr,sizeof(server_addr));
		printf("sent number %d\tnbytes %d\n", n, nbytes);
		sleep(1);
		
}

	close(sock_fd);
	exit(0);
}
