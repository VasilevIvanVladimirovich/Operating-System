#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int sigint_signal = 0;
int sigusr1_signal = 0;
int sigusr2_signal = 0;

void reading_sigint(int Entered_Signal);
void reading_sigusr1(int Entered_Signal);
void reading_sigusr2(int Entered_Signal);
int main()
{
    signal(SIGINT, reading_sigint);
    signal(SIGUSR1, reading_sigusr1);
    signal(SIGUSR2, reading_sigusr2);
    signal(SIGTERM, SIG_IGN);
    while (1)
    {
        time_t Time_Counter=time(NULL);
        usleep(1000);
        while (sigint_signal < 5  && time(NULL) - Time_Counter < 1)
        usleep(1000);
        printf("Time: %ld\n",time(NULL));
        printf("SIGUSR1: %d\n", sigusr1_signal);
        printf("SIGUSR2: %d\n", sigusr2_signal);
        printf("SIGINT: %d\n", sigint_signal);
        if (sigint_signal >= 5) 
        {
			printf("\nThe program was successfully completed.\n");
			break;
		}
    }
}
void reading_sigint(int Entered_Signal)
{
    if (Entered_Signal == SIGINT)
        sigint_signal++;
}
void reading_sigusr1(int Entered_Signal)
{
    if (Entered_Signal == SIGUSR1)
        sigusr1_signal++;
}
void reading_sigusr2(int Entered_Signal)
{
    if (Entered_Signal == SIGUSR2)
        sigusr2_signal++;
}
