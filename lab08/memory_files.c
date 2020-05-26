#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argv, char *argc[])
{
    if (argv != 3)
    {
        printf("Error: the input file and output file names were not entered when the program was started\n");
        return -1;
    }
    int input_file = open(argc[1], O_RDONLY, 0600);
    int output_file = open(argc[2], O_RDWR | O_CREAT, 0600);

	 if (output_file < 0)
    {
        printf("Error output file");
        return -1;
    }
    if (input_file < 0)
    {
        printf("Error input file");
        return -1;
    }
    struct stat buf;
    if((stat(argc[1], &buf))!=0)
    {
		printf("Error stat\n");
        return -1;
		
	}
    ftruncate(output_file, buf.st_size);

    char *data = (char *)mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, output_file, 0);
    if (MAP_FAILED == data)
    {
        printf("Error mmap");
        return -1;
    }

    read(input_file, data, buf.st_size);

    if (0 != munmap(data, buf.st_size))
    {
        printf("Error munmap");
        return -1;
    }
    close(input_file);
    close(output_file);
    printf("The program worked successfully!!!");
    return 0;
}
