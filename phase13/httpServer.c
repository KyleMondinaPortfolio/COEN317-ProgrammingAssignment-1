#include "httpServer.h"

void serveClient(int connfd, const char *fileLocation){

	FILE *filePtr = fopen(fileLocation,"r");
	if (NULL == filePtr){
		perror("Server Error: failed to open requested file\n");
		return;
	}
	int fd = fileno(filePtr);
	printf("Server Success: succesfully opened requested file:%d\n",fd);

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
}

