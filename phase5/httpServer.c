#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#define PORT 80
#define MAX 8000
#define BUFFER_SIZE 8000

void serveClient(int connfd, const char *fileLocation){

	FILE *filePtr = fopen(fileLocation,"r");
	if (NULL == filePtr){
		printf("file cannot be opened\n");
	}
	int fd = fileno(filePtr);
	printf("server succesfully opened requested file:%d\n",fd);

	struct stat st;
	fstat(fd,&st);
	off_t file_size = st.st_size;

	char file_buff[BUFFER_SIZE];

	int n;
	do{
		bzero(file_buff, BUFFER_SIZE);
		n = fread(file_buff,1,BUFFER_SIZE,filePtr);
		if (n>0){
			write(connfd, file_buff, n);
		}
	}while(n>0);
	fclose(filePtr);
	
//	fread(file_buff,sizeof(char),file_size+1,filePtr);
//	fclose(filePtr);
//	write(connfd, file_buff, file_size);
}

void handleHttpRequest(int connfd){
	
	char response[9000] = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1000\n\n";
	char responseHeader[1000] = "HTTP/1.1 200 OK\nContent-Type: ";
	char *jpg = "image/jpg";
	char *html = "text/html;charset=UTF-8";
	char *contentLength = "\nContent-Length: 100000000\n\n";
	strcat(responseHeader, jpg);
	strcat(responseHeader, contentLength);
	write(connfd, responseHeader, strlen(responseHeader));
	serveClient(connfd,"./output-onlinejpgtools.jpg");
	return;

}



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
	
	
	int port = 1025;
	int sockfd = startServerAtPort(port);
	
	//client connection information
	int connfd;
	struct sockaddr_in clientAddr;
	socklen_t alen;

	//accept the client
	if ((connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &alen))<0){
		perror("unable to accept client\n");
		exit(1);
	}
	printf("Server accepted client %d\n",connfd);

	handleHttpRequest(connfd);
	close(sockfd);
	return 0;
	
}
