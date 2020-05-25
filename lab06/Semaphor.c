#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>

void Create_Random_Array(int *mem,int min, int max,int n);

void sem(int semId, int n, int d);

void unlock_Sem(int semId, int n);

void lock_Sem(int semId, int n);

int get_Sem(int semId, int n);

void Sort_Array(int semId, int* mem, const size_t n);

int main(int argv, char *argc[])
{
	if (argv != 4)
	{
		printf("Not supported arguments count\n");
		return 0;
	}
	int n = atoi(argc[1]);
	int min = atoi(argc[2]);
	int max = atoi(argc[3]);
	
	int memId;  
	if ((memId = shmget(IPC_PRIVATE, sizeof(int)*n, 0600|IPC_CREAT|IPC_EXCL))<0){
	
	printf("Can\'t get memId\n");
	exit(-1);
}
	int *mem = (int *)shmat(memId, 0, 0); 
	srand(time(NULL));
	Create_Random_Array(mem,min,max,n);
	printf("Create Array: \n");
	int semId;
	if ((semId= semget(IPC_PRIVATE, n, 0600 | IPC_CREAT))<0)
	 {
		 printf("Can\'t get semid\n");
		 exit(-1);
	}
	for (int i=0; i<n; i++)
	unlock_Sem(semId, i);
	int Process_child_id=fork();
	if(Process_child_id<0)
	{
		printf("Can\'t fork child\n");
		 exit(-1);
	}
	if (Process_child_id) //dad
	{
		int i = 0;
        int status;
        do
        {
            printf("%d: ", i);
             i++;
            for (int j = 0; j < n; j++)
            {
                int Status_Sem = get_Sem(semId, j);
                lock_Sem(semId, j);
                if (Status_Sem)
                {
                    printf("%d ", mem[j]);
                }
                else
                {
                    printf("[%d] ", mem[j]);
                }

                fflush(stdout);
                unlock_Sem(semId, j);
            }
            printf("\r\n");
            status = waitpid(Process_child_id, NULL, WNOHANG);
        } while (!status);
        printf("Sorted Array\r\n");
        for(int j=0; j<n; j++) printf("%d ", mem[j]);
        printf("\r\n");
        shmctl(memId, 0, IPC_RMID); 
        semctl(semId, 0, IPC_RMID); 
        	}
	else
	{
		Sort_Array(semId, mem, n); //child
	}
}

void Create_Random_Array(int *mem,int min, int max,int n)
{
	for (int i=0; i<n; i++)
		mem[i] = rand() % (max - min + 1) + min;
	
}
int get_Sem(int semId, int n)
{
	return semctl(semId, n, GETVAL, 0); 
}

void unlock_Sem(int semId, int n)
{
    sem(semId, n, 1);
}

void lock_Sem(int semId, int n)
{
    sem(semId, n, -1);
}
void sem(int semId, int n, int d)
{
    struct sembuf mybuf;
    mybuf.sem_num = n;
    mybuf.sem_op = d; 
    mybuf.sem_flg = 0;
    semop(semId, &mybuf, 1);
}
void Sort_Array(int semId, int* mem, const size_t n)
{
    for (int i = 0; i < n; i++)
    {
        int minInd = i;
        for (int j = i + 1; j < n; j++)
        {
            lock_Sem(semId, minInd);
            lock_Sem(semId, j);
			int to_unlock = minInd;
            if (mem[j] < mem[minInd])
            {
                minInd = j;
            }
            unlock_Sem(semId, j);
            unlock_Sem(semId, to_unlock);
        }
        if (i != minInd)
        {
            lock_Sem(semId, i);
            int t = mem[i];
			lock_Sem(semId, minInd);
            mem[i] = mem[minInd];
			unlock_Sem(semId, i);
            mem[minInd] = t;
            unlock_Sem(semId, minInd);
        }
    }
}
