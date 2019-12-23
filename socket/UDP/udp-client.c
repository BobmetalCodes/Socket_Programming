/*
UDP based Echo Server Application
*/

#include <stdio.h>
/*
Socket Header files
*/
#include <arpa/inet.h>  //inet_addr
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
	int sockfd, portnumber, clilen, recvbytes; 
		
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
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(portnumber);


	int retcon = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));


	if (retcon < 0)
	{
		error("Connection to server Socket Failed");
		exit(1);
	}
	else {
 		 printf("Connected to Server successfully \n");

	}

	while(1)
	{
        clilen = sizeof(cli_addr);
        bzero(buffer, sizeof(buffer));	
        printf("Enter the Msg to server: ");
        int buffer_count = 0;
        while((buffer[buffer_count++] =getchar()) != '\n');
        sendto(sockfd, buffer, MAX_SIZE, 0 , (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		
        bzero(buffer, sizeof(buffer));	
		recvbytes = recvfrom(sockfd,buffer,MAX_SIZE,0,(struct sockaddr*)&cli_addr, &clilen);
		printf("The msg from Server : %s\t", buffer);

	}

	close(sockfd);
}
