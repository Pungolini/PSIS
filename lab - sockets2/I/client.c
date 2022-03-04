#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "sock_dg.h"


#define RANGE 101
#define serverPort 22



int n;


void *send_thread_func( void * arg)
{
	int sock_fd = *(int*)arg;
	int nbytes = recv(sock_fd, &n, sizeof(n), 0);
	printf("Prime number %d received\tnbytes %d\n", n, nbytes);
}



int main()
{
	pthread_t tid;
	struct sockaddr_in server_addr;
	struct sockaddr_in foreignAddr;
	int nbytes,error,td_err,sock_fd;
	char linha[1000],path[200] = "/tmp/", pid[100];
	char serverIP[] = "194.210.157.7";
	


	/* Start random seed generator */
	srandom(getpid());


	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1);
	}

	foreignAddr.sin_family = AF_INET;
	//foreignAddr.sin_addr = inet_addr(serverIP); /* Server IP address*/
	foreignAddr.sin_port= htons(serverPort);

	inet_aton(serverIP,&foreignAddr.sin_addr);
	// path = "/tmp/" + getpid
	/*sprintf(pid,"%d",getpid());
	strcat(path,pid);
	printf("path = %s\n",path);

	strcpy(foreignAddr.sin_path, path);*/


	// Thread
	if(td_err = pthread_create(&tid, NULL,send_thread_func, (void*)&sock_fd) != 0)
	{
		puts("Error creating Thread...");
		exit(td_err);
	} 

	/* Try connecting to server */
	if((error = connect(sock_fd, (struct sockaddr *)&foreignAddr, sizeof(foreignAddr))) == -1)
	{
		perror("bind");
		exit(-1);
	}

	printf(" Socket created \n Ready to send\n");

	server_addr.sin_family = AF_INET;
	//strcpy(server_addr.sin_path, SOCK_ADDRESS);
	while(1)
	{
		// Gerar numero aleatorio
		n = random()%RANGE;

		// Mandar para o servidor
		nbytes = send(sock_fd, &n, sizeof(n), 0);
		printf("sent number %d\tnbytes %d\n", n, nbytes);

		nbytes = recv(sock_fd, &n, sizeof(n), 0);
		printf("received from SERVER the number %d\n", n);
		sleep(1);
		
	}

	close(sock_fd);
	exit(0);
}
