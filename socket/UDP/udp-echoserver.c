/*
UDP based Echo Server Application
*/

#include <stdio.h>
/*
Socket Header files
*/
#include <sys/types.h>          
#include <sys/socket.h>
#include <strings.h>		
#include <stdlib.h>	
#include <netinet/in.h>	
#include <unistd.h>
#define MAX_SIZE 1024
void error(char *msg)
{
	perror(msg);
	exit(1);
}

/* main function - driver code */
int main(int argc, char *argv[])
{	
	int sockfd, portnumber, recvbytes, clilen;
		
	char buffer[MAX_SIZE];

	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2)
	{
		printf("Error !! Enter Port Number\n");
		exit(1);
	}

	sockfd = socket(AF_INET,SOCK_DGRAM, 0);

	if(sockfd < 0)
	{
		error("Socket is not created");
		exit(1);
	}

	else {
		printf("Socket is created \n");
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));


    portnumber = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portnumber);


	int ret = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));


	if (ret < 0)
	{
		error("Error in binding with the address");
		exit(1);
	}
	else {
 		 printf("Server Started \n");

	}

	while(1)
	{
		clilen = sizeof(cli_addr);
		recvbytes = recvfrom(sockfd,buffer,MAX_SIZE,0,(struct sockaddr*)&cli_addr, &clilen);
		printf("The msg from Client : %s\t", buffer);

		sendto(sockfd, buffer, MAX_SIZE, 0 , (struct sockaddr*)&cli_addr, (socklen_t)clilen);
	}

	close(sockfd);
}
