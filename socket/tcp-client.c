#include<stdio.h>
#include<string.h>
#include<strings.h>

#include<stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#define PORT 9000

#define BUFFER_SIZE 1024
//error handler
void error(char *msg)
{
perror(msg);
exit(1);
}

int main(int argc,char *argv[])
{
	char* disconnect = "bye";
	int sockfd; ///acceptfd;
	struct sockaddr_in serv_addr,cli_addr;
	char buffer[BUFFER_SIZE];
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
	error("socket creation failed");	
	}

	else
	{
	printf("socket created succesfully\n");
	}
	

	bzero(&serv_addr,sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	
	serv_addr.sin_addr.s_addr =  inet_addr("127.0.0.1");

	serv_addr.sin_port = htons(PORT);

	int retcon = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	if(retcon<0)
	{
		error("connection to socket server failed");
	}
	else
	{
		printf("connected to server successfully\n");
	}

	while(1)
	{
		bzero(buffer, sizeof(buffer));
		printf("Enter Msg: ");
		int buffer_count = 0;
		while((buffer[buffer_count++] = getchar())!='\n');
		buffer[buffer_count] = '\0';
		//write data in buffer to server

		write(sockfd, buffer, sizeof(buffer));
		if(!(strncmp(disconnect, buffer,3)))
		{
			printf("Client shutting down");
			break;
		}
		bzero(buffer, sizeof(buffer));

		//Read data in buffer from server

		read(sockfd, buffer, sizeof(buffer));
		printf("Data from server : %s", buffer);

	}

	//close the socket file descriptor
	
	close(sockfd);
}
