#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "sock_dg.h"

#define MAXCLIENT 5
#define serverPort 22

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


int main(){
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t size_client_addr;
	int nbytes,n,fork_id,client_fd;


	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_fd == -1)
	{
		perror("socket: ");
		exit(-1);
	}


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = serverPort;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	strcpy(server_addr.sin_path, SOCK_ADDRESS);
	
	if((err = bind(sock_fd, (struct sockaddr *)&server_addr,sizeof(server_addr))) ==  -1)
	{
		perror("bind");
		exit(-1);
	}

	puts("\nSocket created and binded \n ");

	/* Listen for incoming connections */
	listen(sock_fd,MAXCLIENT);
	puts("Listening...\n");







	while(1)
	{
		//recv from need the size of the address !!!
		size_client_addr = sizeof(struct sockaddr_storage);

		/* Accepts a connection */
		client_fd = accept(sock_fd,(struct sockaddr *) &client_addr,&size_client_addr);

		// Receive from client
		nbytes = recv(client_fd, &n, sizeof(n), 0);

		printf("received from %s the number %d\n", client_addr.sin_path, n);

		if (isPrime(n))
		{
			nbytes = send(sock_fd, &n, sizeof(n), 0);
			printf("sent %d bytes\n", nbytes);
		}

	}
		}

	
	close(client_fd);
	//unlink(SOCK_ADDRESS);
	exit(0);
}
