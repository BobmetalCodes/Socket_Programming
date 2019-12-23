/*
 *	CLIENT-SIDE CODE
 *   --> Bi-Directional <--
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

//socket hearder files
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 1024		//buffer for read and write operations
#define PORT 4000 			//port number of listning server 

/* error handling routine */
void error(int temp,char *str1, char *str2)
{
	if(temp<0)
	{
		perror(str1);
		exit(1);
	}
	else
		perror(str2);
}

int main (int argc, char *argv[])
{
	int sockfd;
	char buffer[BUFFER_SIZE];
	
	/* Send and Receive Buffer */
	char sendbuffer[1024];
	char recvbuffer[1024];

	//server structure declaration 
	struct sockaddr_in serv_addr, cli_addr;

	//Creation of socket 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);						//SOCKET
	error(sockfd,"Socket creation failed ","Socket creation Successful !");
	
	//Clear/flush the structure memory
	bzero(&serv_addr, sizeof(serv_addr));

	//Assigning IP address and PORT 
	serv_addr.sin_family = AF_INET ;

	//refer man 3 inet_addr
	//inet_addr : converts the IPv4 dotted decimal format into the binary (network byte order)
	
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);

	//connect the client socket to the server
	int retcon = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));	//CONNECT
	error(retcon,"Connection with server socket failed","Connection with the server socket Successful !");
	
        /* fork system call to create child process */
        pid_t cpid;
        cpid = fork();

        if (cpid==0)
        {
                while(1)
                {
                        bzero(&sendbuffer,sizeof(sendbuffer));
                        //send data from the client and print
                        printf("\nSend Message : ");
                        fgets(sendbuffer, sizeof(sendbuffer), stdin);
                        send(sockfd, sendbuffer, strlen(sendbuffer)+1, 0);              //SEND
                        printf("\nMessage Sent Successfully");
                }
                close(sockfd);
        }
        else
        {
                while(1)
                {
                        bzero(&recvbuffer,sizeof(recvbuffer));
                        //receive data from the client and print
                        recv(sockfd, recvbuffer, sizeof(recvbuffer), 0);                //RECEIVE
                        printf("\nClient Message : %s\n", recvbuffer);
                        bzero(recvbuffer, sizeof(recvbuffer));
                }
                close(sockfd);
        }

        return 0;
}
