#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 80
#define MAX 8000
#define BUFFER_SIZE 8000

void spprint(){
	FILE *ptr;
	char ch;
	ptr = fopen("test.txt", "r");
	if (NULL == ptr){
		printf("file cannot be opened\n");
	}
	printf("contents are \n");
	do {
		ch = fgetc(ptr);
		printf("%c", ch);
	}while(ch!=EOF);
	fclose(ptr);
	return;
}

void printFileContents(){
	FILE * filePtr;
	char fileContents[BUFFER_SIZE];
	bzero(fileContents,BUFFER_SIZE);
	char ch;
	filePtr = fopen("./test.txt","r");
	if (NULL == filePtr){
		printf("file cannot be opened\n");
	}
	printf("contents of file are: \n");

	while(!feof(filePtr)){
		ch = fgetc(filePtr);
		printf("%c", ch);
		strncat(fileContents,&ch,1);
	}

	fclose(filePtr);
	printf("the contnets are\n");
	printf("%s\n", fileContents);
	return;
}
void handleHttpRequest(int connfd){
	
	char dataBuffer[BUFFER_SIZE];
	char *httpMethod;
	char *requestedFile;
	int n;

	char response[9000] = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1000\n\nHello";

	FILE * filePtr;
	char fileContents[BUFFER_SIZE];
	bzero(fileContents,BUFFER_SIZE);
	char ch;
	filePtr = fopen("./test.txt","r");
	if (NULL == filePtr){
		printf("file cannot be opened\n");
	}
	printf("contents of file are: \n");

	while(!feof(filePtr)){
		strncat(fileContents,&ch,1);
		ch = fgetc(filePtr);
	}

	fclose(filePtr);
	printf("%s\n", fileContents);
	strcat(response, fileContents);
	
	write(connfd, response, strlen(response));
	return;

	/*for(;;){
		//reset the buffer
		bzero(dataBuffer,BUFFER_SIZE);
		//put the data data from the client to the dataBuffer
		read(connfd,dataBuffer,sizeof(dataBuffer));
		//parse the http request
		httpMethod = strtok(dataBuffer, " ");	
		requestedFile = strtok(NULL, " ");

		printf("%s\n", httpMethod);
		printf("%s\n", requestedFile);
		write(connfd, response, sizeof(response));
		
		n = 0;
		//copy server message in the buffer
		bzero(dataBuffer, BUFFER_SIZE);
		while((dataBuffer[n++] = getchar()) != '\n');
		//send to client

		//to exit of the program
		if (strncmp("exit", dataBuffer, 4) == 0){
			printf("Server Exit ... \n");
			break;
		}
	

	}*/
}


int main(int argc, char *argv[]){
	
	
	spprint();
	int port = 1025;	
	int sockfd;

	//Open a tcp socket, if successful, returns the file descriptor associated with the endpoint
	//AF_INET, SOCK_STREAM, and socket are defined inside stdlib.h
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("unable to open a tcp socket\n");
		exit(1);
	}

	//allow for immediate use of the port
	int sockoptval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

	//bind the tcp socket using the sockaddr_in structure defined in netint/in.h for IP networking
	struct sockaddr_in serverAddr;
	
	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	//set the address family we use when setting up the socket, which is AF_INET for IP Networking
	serverAddr.sin_family = AF_INET;
	//set the address for this socket, we are using the special address 0.0.0.0 defined by the symbolic
	// INADDR_ANY to let the operating system use whatever interface it wants
	// htonly means host to network long: which converts a number into a 32-bit network representation
	// commonly used in storing an IP address to a sockaddr structure
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//set up the address port number
	//htons means host to netowrk long: convert a number into a 32-bit network represntatinon.
	//commonly used to store an socket port number into a sockaddr structure
	serverAddr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))<0){
		perror("binding of tcp socket failed\n");	
		exit(1);
	}
	printf("Binded\n");


	//accept the connection with a queue backlog of 5
	if (listen(sockfd,5)<0){
		perror("listening failed\n");
		exit(1);
	}
	printf("Listening to socket\n");

	//the sockfd of the client
	int connfd;
	//the address of the client that will get filled
	struct sockaddr_in clientAddr;
	//length of the address structure
	socklen_t alen;

	//accept the client
	if ((connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &alen))<0){
		perror("unable to accept client\n");
		exit(1);
	}
	printf("Accepted\n");

		

	//communicateWith(connfd);
	handleHttpRequest(connfd);
	
	close(sockfd);
	return 0;
	
}
