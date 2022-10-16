#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#define MAX 8000
#define BUFFER_SIZE 8000




int startServerAtPort(int port){
	int sockfd;

	//open a tcp socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("unable to open a tcp socket\n");
		exit(1);
	}
	printf("server tcp socket successfully opened\n");

	//allow for immediate use of the port
	int sockoptval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));
	
	//bind the tcp socket to a port
	struct sockaddr_in serverAddr;
	memset((char *)&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))<0){
		perror("binding of tcp socket failed\n");	
		exit(1);
	}
	printf("server tcp socket sucessfully binded to port: %d\n",port);

	//accept the connection with a queue backlog of 50
	if (listen(sockfd,50)<0){
		perror("listening failed\n");
		exit(1);
	}
	printf("server now listening to tcp socket %d\n",sockfd);

	return sockfd;

}


int main(int argc, char *argv[]){

	if (argc != 5){
		printf("incorrect number of arguments given\n");
		printf("number of arguments given: %d\n", argc);
		printf("failed to start the server\n");
		return -1;	
	}

	int port = atoi(argv[4]);	
	const char *document_root = argv[2];

	int sockfd = startServerAtPort(port);
	
	//client connection information
	int connfd;
	struct sockaddr_in clientAddr;
	socklen_t alen;
	
	while(1){

		if ((connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &alen))<0){
			perror("unable to accept client\n");
			exit(1);
		}
		printf("Server accepted client %d\n",connfd);

		struct args * thread_args = (struct args *)malloc(sizeof(struct args));
		thread_args->document_root = document_root;
		thread_args->connfd = connfd;

		pthread_t t;
		int *pclient = malloc(sizeof(int));
		*pclient = connfd;

		pthread_create(&t, NULL,handleRequestThread,(void *)thread_args); 
	}

	return 0;
	
}
