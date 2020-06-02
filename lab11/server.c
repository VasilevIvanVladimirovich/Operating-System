#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int Socket(int domain,int type,int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd,int backlog);

int Accept(int sockfd, struct sockaddr *addr,socklen_t *addrlen);

int comp(const void *a,const void *b);

int main(int argv,char *argc[])
{
	if (argv!=2)
	{
		perror("Not port given");
		exit(EXIT_FAILURE);
	}
	int port = atoi(argc[1]);
	if (port == 0)
	{
		perror ("Not Correct port");
		exit(EXIT_FAILURE);
	}
	
	int server = Socket(AF_INET,SOCK_STREAM,0); //ipv4 tcp
	struct sockaddr_in adr={0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(port);
	if (adr.sin_port ==0)
	{
		printf("The entered port is occupied by another port\n");
		printf("New port: %d\n",port);      
		
	}else printf("Server port: %d \n",port);
	
	Bind(server,(struct sockaddr *) &adr,sizeof adr);
	Listen(server,1); 
	socklen_t adrlen = sizeof adr;
	int fd = Accept(server,(struct sockaddr *) &adr, &adrlen);
	ssize_t nread;
	char buf[256];
	nread=read(fd,buf,256);
	int *Array=(int*)malloc(nread);
	if (nread == -1)
	{
		perror("read failed");
		exit(EXIT_FAILURE);
	}
	if (nread == 0)
	{
		printf("End of file occered:\n");
	}
	
	for(int i=0;i<nread;i++)
	{
		Array[i]=(int)buf[i];
	}
	qsort(Array,nread,sizeof(int),(int(*)(const void*,const void*))comp);
	for(int i=0;i<nread;i++)
	{
		buf[i]=Array[i];
	}
	write(fd,buf,nread);
	free(Array);
	sleep(1);
	close(fd);
	close(server);
	return 0;
}

int Socket(int domain,int type,int protocol)
{
	 int res = socket(domain,type,protocol);
	 if (res == -1){
		 printf("socket failed");
		 exit(EXIT_FAILURE);
	 }
	 return res;
}
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int res = bind(sockfd,addr,addrlen);
	if(res == -1){
		printf("Bind failed");
		exit(EXIT_FAILURE);
	}
}

void Listen(int sockfd,int backlog)
{
	int res = listen(sockfd,backlog);
	if (res == -1)
	{
		printf("listen failed");
		exit(EXIT_FAILURE);
	}
	
}

int Accept(int sockfd, struct sockaddr *addr,socklen_t *addrlen)
{
	int res=accept(sockfd,addr,addrlen);
	if (res == -1)
	{
		printf("accept failed");
		exit(EXIT_FAILURE);
	}
	return res;
}

int comp(const void *a,const void *b)
{
	return *(const int *)b-*(const int *)a;
}


