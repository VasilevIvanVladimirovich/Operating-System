#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

int comp(const int *a, const int *b);
void FunctionSort(int *mem);
void CreatAndPrintArray(int *mem);

int main()
{
	srand(time(NULL));
	int memId = shmget(IPC_PRIVATE, sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
	int *mem = (int *)shmat(memId, 0, 0);
	CreatAndPrintArray(mem);
	printf("Sorted array:\n");
	int sonId;
	sonId = fork();
	if (sonId == 0)
	{
		FunctionSort(mem);
	}
	else
	{ 
		waitpid(sonId, NULL, 0);
	}
	shmdt(mem);
	shmctl(memId, 0, IPC_RMID);
	return 0;
}
void CreatAndPrintArray(int *mem)
{
	printf("Original array:\n");
	for (size_t i = 0; i < 20; i++)
	{
		mem[i] = rand() % 100;
	}
	for (size_t i = 0; i < 20; i++)
	{
		printf("%d ", mem[i]);
	}	
	printf("\n");
}
	
int comp(const int *a, const int *b)
{
	return *a - *b;
}

void FunctionSort(int *mem)
{
	qsort(mem, 20, sizeof(int), comp);
	for (size_t i = 0; i < 20; i++)
	{
		printf("%i ", mem[i]);
	}
	printf("\n");
}
