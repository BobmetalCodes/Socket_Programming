/*
 *	CLIENT-SIDE CODE
 *   --> Bi-Directional <--
 */

#include<pthread.h>
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

/* Global Definitions and declarations*/
	pthread_t t1,t2;

    int sockfd;
	char buffer[BUFFER_SIZE];
	
	/* Send and Receive Buffer */
	char sendbuffer[1024];
	char recvbuffer[1024];

	//server structure declaration 
	struct sockaddr_in serv_addr, cli_addr;


/* End global decl and def*/


/****************** thread method 1 for receive ******************/

void *th1(void *arg) {
        while(1)
                {
                        bzero(&sendbuffer,sizeof(sendbuffer));
                        //send data from the client and print
                        printf("\nSend Message : ");
                        fgets(sendbuffer, sizeof(sendbuffer), stdin);
                        send(sockfd, sendbuffer, strlen(sendbuffer)+1, 0);              //SEND
        //                printf("\nMessage Sent Successfully");
                }
                close(sockfd);
}

/***************** thread method 2 for receive ******************/
void *th2(void *arg){
        while(1)
        {
            bzero(&recvbuffer,sizeof(recvbuffer));
            //receive data from the client and print
            recv(sockfd, recvbuffer, sizeof(recvbuffer), 0);                //RECEIVE
            printf("\nServer Message : %s\n", recvbuffer);
            bzero(recvbuffer, sizeof(recvbuffer));
         }
         close(sockfd);
}




/************** main function - driver code *************/
int main (int argc, char *argv[])
{
	
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
	
******************Thread Creation**********************/
	int t_stat = pthread_create(&t1, NULL, th1, NULL);
	if(t_stat){
		perror("Thread creation failure!");
		exit(1);
	}
	else
	{
		printf("Thread_1 created successfully\n\n");
	}

	t_stat = pthread_create(&t2, NULL, th2, NULL);
        if(t_stat){
                perror("Thread creation failure!");
                exit(1);
        }
        else
        {
                printf("Thread_2 created successfully\n\n");
        }

	void *stat;
	pthread_join(t1, stat);
	pthread_join(t2, stat);


	return 0;
}
       

        
                
        }
        else
        {
                
        }

        return 0;
}
