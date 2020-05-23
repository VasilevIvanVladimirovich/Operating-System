#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int memId;
	const size_t memSize=200;
	srand(time(NULL));
	
	if((memId=shmget(IPC_PRIVATE, memSize, 0600|IPC_CREAT|IPC_EXCL))<=0){
		printf("error shmget\n");
		return -1;
	}
	printf("shmid=%i\n",memId);
	int *mem=(int*)shmat(memId,0,0);
	if(NULL==mem){
		printf("error shmat()\n");
		return -2;
	}
	for(int i=0;i<20;i++)
	{
		mem[i]=rand()%100;
	}
	char callbuf[1024];
	sprintf(callbuf,"./receiver %i",memId);
	system(callbuf);

	return 0;
	}
