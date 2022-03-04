#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "sock_dg.h"


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
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
	socklen_t size_client_addr;
	int n;
	int nbytes;

	int sock_fd= socket(AF_UNIX, SOCK_DGRAM, 0);

	if (sock_fd == -1){
		perror("socket: ");
		exit(-1);
	}

	unlink(SOCK_ADDRESS);
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_ADDRESS);
	int err = bind(sock_fd, (struct sockaddr *)&server_addr,
							sizeof(server_addr));
	if(err == -1) {
		perror("bind");
		exit(-1);
	}
	printf(" socket created and binded \n ");
	printf("Ready to receive messages\n");
	while(1)
	{
		//recv from need the size of the address !!!
		size_client_addr = sizeof(struct sockaddr_storage);
		//
		nbytes = recvfrom(sock_fd, &n, sizeof(n), 0,
										(struct sockaddr *) &client_addr,
										&size_client_addr);


		printf("received from %s the number %d\n", client_addr.sun_path, n);

		if (isPrime(n))
		{
			n = n*10;
			nbytes = sendto(sock_fd, &n, sizeof(n), 0,(const struct sockaddr *) &client_addr,sizeof(client_addr));
			printf("sent %d bytes\n", nbytes);
		}

	}
	close(sock_fd);
	//unlink(SOCK_ADDRESS);
	exit(0);
}
