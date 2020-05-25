#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

struct msgbuf
{
    long mtype;
    int data[4];
    int islast;
};

int Creat_random(int min, int max);

int comp(const int *a, const int *b);

void swap(int *a, int *b);

int next(int perm[], int n);

int main()
{
    int msgId = msgget(IPC_PRIVATE, 0600 | IPC_CREAT|IPC_EXCL); 
    if (msgId < 0)
    {
        perror("error with msgget()");
        return -1;
    }
    else
    {
        printf("Message id = %i\n", msgId);
    }
    pid_t Process_child_Id = fork();
    if (Process_child_Id < 0)
    {
        perror("error with fork()\n");
    }
    else if (Process_child_Id > 0) 
    {
    struct msgbuf parentmsg;
    srand(time(NULL));
    for (int i = 0; i < 4; i++) parentmsg.data[i] = Creat_random(0, 100);
    printf("Parent Process: generate  %i %i %i %i\n", parentmsg.data[0], parentmsg.data[1], parentmsg.data[2], parentmsg.data[3]);
    parentmsg.islast = 1;
    parentmsg.mtype = 1;
    if ((msgsnd(msgId, &parentmsg, sizeof(parentmsg), 0))!=0) 
    {
		printf("Error in msgsnd(Process_parend)\n");
		return -1;
	}	
    int number_of_permutations = 0;
    do
    {
        if((msgrcv(msgId, &parentmsg, sizeof(parentmsg), 2, 0))==-1) 
        {
			printf("Error in msgrcv(Process_parent)\n");
			return -1;
		}
        if (parentmsg.islast)
        break;
        number_of_permutations++;
        printf("Parent Process: get %i: %i %i %i %i\n", number_of_permutations, parentmsg.data[0], parentmsg.data[1], parentmsg.data[2], parentmsg.data[3]);
    } while (!parentmsg.islast);
    printf("Parent Process: wait until child is finished.\n");
    waitpid(0, 0, 0);
    if((msgctl(msgId, IPC_RMID, NULL))==-1) 
    {
		printf("Error in msgrcv(Process_parent)\n");
		return -1;
	}
    printf("Parent Process: Process is finished.\n");
    printf("Total permutations is: %i\n",number_of_permutations);
    }
    else 									
    {
    struct msgbuf childmsg;
    if ((msgrcv(msgId, &childmsg, sizeof(childmsg), 1, 0))==-1) 
    {
		printf("Error in msgrcv(Process_child)\n");
		return -1;
	}
    printf("Child  Process: data reading complete.\n");
    qsort(childmsg.data, 4, sizeof(int), (int(*)(const void *,const void *))comp);
    childmsg.islast = 0;
    childmsg.mtype = 2;
    if((msgsnd(msgId, &childmsg, sizeof(childmsg), 0))!=0) 
	{
		printf("Error in msgsnd(Process_child)\n");
		return -1;
	}	
    do
    {
        childmsg.islast = !next(childmsg.data, 4);
        childmsg.mtype = 2;
        msgsnd(msgId, &childmsg, sizeof(childmsg), 0); 
    } while (!childmsg.islast);
    printf("Child  Process: Process is finished.\n");
    }
    return 0;
}


int Creat_random(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int comp(const int *a, const int *b)
{
    return *a > *b;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int next(int perm[], int n)
{
    int i = n - 1;
    while (--i >= 0 && perm[i] > perm[i + 1]);
    if (i == -1)
        return 0;
    for (int j = i + 1, k = n - 1; j < k; j++, k--)
        swap(perm + j, perm + k);
    int j = i + 1;
    while (perm[j] < perm[i])
        j++;
    swap(perm + i, perm + j);
    return 1;
}




