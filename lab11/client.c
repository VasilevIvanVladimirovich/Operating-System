#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>


int Socket(int domain,int type,int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *src,void *dst);

void Random_num(int *arr,int size,int min,int max );

unsigned long long mic_s_time();

int main(int argv,char *argc[])
 {
	 if (argv != 2)
	 {
		 perror("No Address given");
		 exit(EXIT_FAILURE);
	 }
	 int border=strstr(argc[1],":")-argc[1];
	 if(border+argc[1]==NULL)
	 {
		 perror("Not correct format adress");
		 exit(EXIT_FAILURE);
	 }
	 
	 
	 unsigned long long t;
	 int size_Arr,Max,Min;
	 
		char ip[border+1];
		strncpy(ip,argc[1],border);
		ip[border]='\0';
		int port = atoi(argc[1]+border+1);
		
	 int fd = Socket(AF_INET,SOCK_STREAM,0);
	 struct sockaddr_in adr = {0};
	 adr.sin_family=AF_INET;
	 adr.sin_port = htons(port);
	 Inet_pton(AF_INET,ip,&adr.sin_addr);
	 Connect(fd,(struct sockaddr *)&adr,sizeof adr);
	 
	 printf("Enter size elements: ");
	 scanf("%d",&size_Arr);
	 printf("\nEnter minimum value: ");
	 scanf("%d",&Min);
	 printf("\nEnter maximum value: ");
	 scanf("%d",&Max);
	 printf("\n");
	 
	 int *Array =(int *)malloc(size_Arr);
	 char *Arr = (char *)malloc(size_Arr);
	 Random_num(Array,size_Arr,Min,Max);
	 printf("Array Create:");
	 for(int i=0;i<size_Arr;i++)
	{
		Arr[i]=Array[i];
		printf("%d ",Arr[i]);
	}
	printf("\n");
	 t=mic_s_time();
	 
	 write(fd, Arr, size_Arr);
	 char buf[size_Arr];
	 ssize_t nread;
	 nread = read(fd,buf,size_Arr);
	 
	 t=mic_s_time()-t;
	 
	 if(nread == -1)
	 {
		 printf("read failed");
		 exit(EXIT_FAILURE);
	 }
	 if(nread==0)
	 {
		 printf("EOF occured\n");
	 }
	 printf("Array from server: ");
	 for(int i=0;i<size_Arr;i++){
	
		printf("%d ",buf[i]);
	}
	printf("\n");
	printf("The wait time from the server: %lld ms\n", t);
	free(Array);
	free(Arr);
	 close(fd);
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


void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int res = connect(sockfd,addr,addrlen);

	if (res == -1)
	{
		printf("Connect faild");
		exit(EXIT_FAILURE);
	}
	
}

void Inet_pton(int af, const char *src,void *dst)
{
	int res = inet_pton(af,src,dst);
	if (res == 0){
		printf("inet_pton faild, valid network addres in the specified addres family\n");
		exit(EXIT_FAILURE);
	}
	if (res == -1)
	{
		printf("Inet_pton faild");
		exit(EXIT_FAILURE);
	}
}

void Random_num(int *arr,int size,int min,int max )
{
	srand(time(NULL));
	for(int i=0;i<size;i++)
	{
		arr[i]=rand()% (max - min + 1) + min;
	}
}

unsigned long long mic_s_time()
{
	struct timeval t;
	gettimeofday(&t,NULL);
	unsigned long long mt=(unsigned long long)t.tv_sec*1000000+t.tv_usec;
	return mt;
}
