#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define BUFFER_SIZE 8000

typedef struct Thread_args {
	char *document_root;
	int connfd;
} Thread_args;

void serveFile(int connfd, const char *fileLocation);
void *handleRequestThread(void *thread_args);

void *handleRequestThread(void *args){

	//parsing supplied arguments
	Thread_args *thread_args = (Thread_args*) args;
	char *document_root = thread_args->document_root;
	int connfd = thread_args->connfd;


	//first parse the http header sent by the client
	char requestBuffer[BUFFER_SIZE];
	bzero(requestBuffer,BUFFER_SIZE);
	read(connfd, requestBuffer, sizeof(requestBuffer));

	char *httpMethod = strtok(requestBuffer, " ");	
	char *requestedFile = strtok(NULL, " ");

	//check for any bad requests
	if(strncmp(requestedFile,"//",2)==0){
		char responseHeader[1000] = "HTTP/1.0 400 Bad Request\nContent-Type: text/html;charset=UTF-8\nContent-Length: 100000000\n\n";
		write(connfd, responseHeader, strlen(responseHeader));
		char formattedFile[BUFFER_SIZE];
		strcat(formattedFile,document_root);
		strcat(formattedFile,"/http400.html");
		serveFile(connfd,formattedFile);
		close(connfd);
		pthread_exit(NULL);
		return NULL;

	}

	//first handle the default / loading page
	if(strcmp(requestedFile,"/")==0){
		char responseHeader[1000] = "HTTP/1.0 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: 100000000\n\n";
		write(connfd, responseHeader, strlen(responseHeader));
		char formattedFile[BUFFER_SIZE];
		strcat(formattedFile,document_root);
		strcat(formattedFile,"/index.html");
		serveFile(connfd,formattedFile);
		close(connfd);
		pthread_exit(NULL);
		return NULL;
	}

	//check the requested file
	char formattedFile[BUFFER_SIZE];
	strcat(formattedFile,document_root);
	strcat(formattedFile,requestedFile);
	printf("requested file is: %s\n", formattedFile);
	char statusCode[40];


#endif 
