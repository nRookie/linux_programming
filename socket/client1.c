#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main()
{
	int sockfd;
	int len;
	struct sockaddr_in address;

	int result;
	char ch = 'A';

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_port = htons(9734);
	inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);
	len = sizeof(address);
	result = connect(sockfd,(struct sockaddr *)&address,len);

	if(result == -1){
		perror("oops: client1");
		exit(1);
	}

	write(sockfd,&ch,1);
	read(sockfd,&ch,1);

	printf("char from server = %c\n",ch);
	close(sockfd);
	exit(0);


}
