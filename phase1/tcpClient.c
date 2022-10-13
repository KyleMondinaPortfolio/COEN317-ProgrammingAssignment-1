#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

void communicateWith(int sockfd){
	char buff[80];
	int n;
	for(;;){
		bzero(buff,sizeof(buff));
		printf("Enter the string: ");
		n = 0;
		while((buff[n++] = getchar()) != '\n');

		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
	
		printf("From Server: %s", buff);
		if ((strncmp(buff, "exit", 4))==0){
			printf("Client Exit...\n");
			break;
		}
	}
}

void paddr(unsigned char *a){
	printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

int main(){

	int sockfd;
	int port = 1025;
	
	//host information, the data type returned by gethostbyname
	struct hostent *hp;
	hp = (struct hostent *)gethostbyname("localhost");
	int i;
	for (i=0;hp->h_addr_list[i] != 0; i++){
		paddr((unsigned char*) hp->h_addr_list[i]);
	}
//	hp = gethostbyname("localhost");
	//the server address
	struct sockaddr_in serverAddr;

	//filling the the server's address and data
	//memset((char*)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_addr = *((struct in_addr *)hp->h_addr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	//put the host address inot the server address structure
	//memcpy((void *)&serverAddr.sin_addr, hp->h_addr_list[0], hp->h_length);	

	//open a socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("unable to open a tcp socket\n");
		exit(1);
	}

	//connect to the server
	if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) <0){
		perror("connection failed\n");
		exit(1);
	}

	communicateWith(sockfd);
	close(sockfd);
	return 0;
	
	
}
