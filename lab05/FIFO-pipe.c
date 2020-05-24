#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int comp(const int *a, const int *b);
int *Creat_Array(int n);
void Print_Array(int *Array, int n);
const char *FIFO_NAME = "My_name_is_fifo";

int main(int argv, char *argc[])
{
	
	int n = atoi(argc[1]);
	int *Array = Creat_Array(n);
	printf("Creat Array:\n");
	Print_Array(Array, n);
	if(mknod(FIFO_NAME, S_IFIFO | 0666, 0)<0){
		printf("Can\'t create FIFO\n");
		unlink(FIFO_NAME);
		exit(-1);
		}
	int fd[2];
	if (pipe(fd)<0){
		printf("Can\'t create pipe\n");
		exit(-1);
	}
	int Process_child_id = fork();
	if (Process_child_id < 0)
	{
		printf("Can\'t fork child\n");
		exit(-1);
	}
	else if(Process_child_id > 0) //dad
	{
		close(fd[1]);
		int fifo;
		if((fifo = open(FIFO_NAME, O_WRONLY))<0)
		{
			printf("Can\'t open FIFO for writing\n");
			exit(-1);
		}
		write(fifo, Array, sizeof(int) * n);
		close(fifo);
		int *Sorted_Array = malloc(sizeof(int) * n);
		read(fd[0], Sorted_Array, sizeof(int) * n);
		printf("Sorted Array:\n");
		Print_Array(Sorted_Array, n);
		close(fd[0]); //closed pipe
		unlink(FIFO_NAME); // closed FIFO
		free(Sorted_Array); //deleted dynamic Array
	}
	else if (Process_child_id == 0)//child
	{
		close(fd[0]); 
		int fifo;
		if ((fifo=open(FIFO_NAME, O_RDONLY))<0)
		{
			printf("Can\'t open FIFO for reading\n");
			exit(-1);
		}
		int *Received_Array = malloc(sizeof(int) * n);
		read(fifo, Received_Array, sizeof(int) * n);
		close(fifo);
		qsort(Received_Array, n, sizeof(int), (int(*)(const void *,const void *))comp);
		write(fd[1], Received_Array, sizeof(int) * n);
		close(fd[1]);
		free(Received_Array); //deleted dynamic array
	}
	
	free(Array); //deleted dynamic array
 }
int comp(const int *a, const int *b)
{
    return *b - *a;
}

int *Creat_Array(int n)
{
	srand(time(NULL));
	int *Array = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
	{
		Array[i] = rand() % 100;
	}
	return Array;
}

void Print_Array(int *Array, int n)
{
	for (int i = 0; i < n; i++)
	{	
		printf("%d ", Array[i]);
	}
	printf("\n");
}
