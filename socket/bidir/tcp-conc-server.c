/*
 *	SERVER-SIDE CODE
 *   --> Bi-Directional <--
 *    --> Chat Server <--
 */

/* Socket Header files */
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>		//bzero API(s)
#include <stdio.h>
#include <unistd.h>		//read system call
#include <stdlib.h>		//exit
#include <netinet/in.h>		//structure member

/* Error Handler/routine */
void error(char *msg)
{
	perror(msg);
	exit(1);
}

/* main function - driver code */
int main(int argc, char *argv[])
{
	/* master socket descriptor */	
	int sockfd; 

	/* Socket descriptor for accept system call */
	int connfd; 

	/* server port number and client address len declaration */
	int portnumber, clilen;  

	/* buffer to read and write the data */
	char buffer[1024], bye[1024]="BYE\n";

	/* Send and receive buffers */
	char sendbuffer[1024];
	char recvbuffer[1024];

	/* server/client structure declaration */
	struct sockaddr_in serv_addr, cli_addr;

	/* read bytes from  server */	
	int readbytes;

	/* condition check for commandline argument*/
	if (argc < 2) {
		printf("Error !! Enter Port Number\n");
		exit(1);
	}

	/*
	   Creation of Master Socket for bind, listen and accept system calls
	   -> refer man socket(2) for details
		AF_INET - IPv4 Internetprotocols         
		SOCK_STREAM - TCP based stream 
		0 - protcol (decided by system/kernel/OS)
 	*/
	sockfd = socket(AF_INET,SOCK_STREAM, 0);					//SOCKET
		if(sockfd < 0) { error("Socket is not created"); }

	/* clear the structure buffer with either bzero or memset */
	bzero((char *)&serv_addr, sizeof(serv_addr));

	/* port number - integer - atoi()) entred from command line ./filename <portnumber> */
	portnumber = atoi(argv[1]);

	/* Initialize the sturucture members */
	serv_addr.sin_family = AF_INET; 	//IPV4 Internet family

	/* INADDR_ANY - binds to all the available interfaces of host machine/work station
			else specific interfaces can be initialized like :
			localhost
			127.0.0.1
			Wifi - interface - eg. "192.168.75.1"

	  layman terminology : client can connect to server with all interfaces of server
	*/
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	/* htons - host to network byte order(short) network byte order - BigEndian */
	serv_addr.sin_port = htons(portnumber);


	/* bind the socket with server address with bind system call */
	int ret = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));	//BIND
		if (ret < 0) {	error("Error in bindind"); }


	/* listen to maximum backlog connections --> listen system call */
	int backlog = 5;
	int ret1 = listen(sockfd, backlog);						//LISTEN
		if(ret1 < 0) { error("listen failed to accept requests"); }

	clilen = sizeof(cli_addr);
	connfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);		//ACCEPT
		if (connfd < 0 ) { error("error in Accepting the request"); }

	
	/* fork system call to create child process */
	pid_t cpid;
	cpid = fork();

	if (cpid ==0)
	{
		while(1)
		{
			bzero(&recvbuffer,sizeof(recvbuffer));
			//receive data from the client and print
			recv(connfd, recvbuffer, sizeof(recvbuffer), 0);		//RECEIVE
			printf("\nClient Message : %s\n", recvbuffer);
		        bzero(recvbuffer, sizeof(recvbuffer));
		}
		close(sockfd);
	}
	else
	{
		while(1)
		{
			bzero(&sendbuffer,sizeof(sendbuffer));
			//send data from the client and print
			printf("\nSend Message : ");
			fgets(sendbuffer, sizeof(sendbuffer), stdin);
			send(connfd, sendbuffer, strlen(sendbuffer)+1, 0);		//SEND
			printf("\nMessage Sent Successfully");

		}
		close(sockfd);
	}

	return 0;
	
}
