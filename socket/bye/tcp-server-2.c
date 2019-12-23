#include<stdio.h>
#include<stdlib.h>
///socket header files
#include<sys/types.h>
#include<sys/socket.h>
#include <string.h>
#include<unistd.h>
#include<netinet/in.h>

//error handler
void error(char *msg)
{
perror(msg);
exit(1);
}

///main function
int main(int argc,char *argv[])
{
	char* disconnect = "bye";
	int txflag=0,rxflag=0;
	int sockfd; ///master socket descrypter
	int connfd; //socket descrypter for accept system call
	int portnumber,clilen; //server port number and client address length declaration
	char buffer[1024]; //buffer to read and write data
	struct sockaddr_in serv_addr,cli_addr; ///client/server structure declaration
	int readbytes; //read bytes from server
	
//condition check for command line
 	if(argc<2)
	{
	printf("error in port number");
	exit(1);	
	}
//creation for master socket  to bind,listen and accept system call
//REFER MAN 2 SOCKET FOR DETAILS AF)INET-IPv4 PROTOCOLS
//SOCK_STREAM FOR SELECTING TCP PROTOCOL
//0- PROTOCOL (DECIDED BY SYSTEM/KERNEL/OS)
	sockfd=socket(AF_INET,SOCK_STREAM,0);
 	if(sockfd<0)
	{
	perror("socket is not created");
	exit(1);
	}
	else {
		printf("Socket created successfully\n");
	}

///clear the structure buffer with either bzero or memset
bzero((char *)&serv_addr,sizeof(serv_addr));

//port number - integer - atoi()(ASCII TO INTEGER CONVERSION)
	portnumber=atoi(argv[1]);

//initialize the structure member
	serv_addr.sin_family=AF_INET; //ipV4 internet family
	serv_addr.sin_addr.s_addr = INADDR_ANY;	//INADDR_ANY - BINDS TO ALL THE AVAILABLE INTERFACE OF HOST MACHINE/WORKSTATION 
///ELSE SPECIFIC INTERFACE CAN BE INTIALIZED LIKE:localhost(127.0.0.1)
//wifi - interface - eg "192.168.75.1"
//layman terminology client can connect to server with all interfaces of server

//htons  - host to neterk byte orde(short) netwrk byte order - bigindian
	serv_addr.sin_port = htons(portnumber);

//bind the socket with server address with bind system call
	int ret= bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	if(ret<0)
	{
	error("error in binding");
	exit(1);
	}
	else { printf("Bind successful\n");}

//listen to max backlog connection 
	int backlog = 5;
	int ret1 = listen(sockfd,backlog);
	if(ret1<0)
	{
	error("listen failed to accept request");
	exit(1);
	}

 	clilen = sizeof(cli_addr);
	

//accept system call to fetch pending request from the queue
	connfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);
	if(connfd<0)
	{	
	error("error in accepting the request");
	exit(1);
	} 
	else { printf("accepted connection\n");}	
	
	while(1)
	{
		//clear buffer 
		bzero(buffer,sizeof(buffer));

		//read data from the client end
		read(connfd,buffer,sizeof(buffer));

		if(!(strncmp(buffer, disconnect,3)))
		{
			printf("Server stopping.");
			break;
		}
		
		printf("Data from client: %s",buffer);
		//write data to client 
		printf("Enter Msg: ");
        	bzero(buffer, sizeof(buffer));
        	int buffer_count = 0;
       	
		while((buffer[buffer_count++] = getchar())!='\n');
		write(connfd, buffer, sizeof(buffer));


	}	
	close(sockfd);
}

