#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argv, char *argc[])
{
    if (argv != 3)
    {
        printf("Error,the file name and number of bytes to read were not entered\n");
        return -1;
    }
	printf("File name: %s \n",argc[1]);
    int Byte = atoi(argc[2]);
    printf("Entered Read Bytes: %d \n", Byte);

    FILE *file_open = fopen(argc[1], "r");
    if (file_open ==NULL )
    {
        printf("Error opening the file");
        return -1;
    }
	size_t Number_of_read=0;
	int Code_Symbols;
    printf("read result: \n");
    printf("Number\t\tCode\t\tSymbols\n");    
    for (int i = 0; i < Byte; i++)
    {
        char data;
        Number_of_read =Number_of_read + fread(&data, sizeof(char), 1, file_open);
        Code_Symbols=(int)(data); 
        printf("%d \t\t (0x%x) \t\t %c \n",i+1,Code_Symbols,data);
    }
   
    printf("\n");
    printf("Read symbols =%ld\n", Number_of_read);
    fclose(file_open);
    printf("The programm worked successfully\n");
    printf("\n");
    char v_consol[50]="";
    strcat(v_consol,"dd ");
    strcat(v_consol,"if=");
    strcat(v_consol,argc[1]);
    strcat(v_consol," bs=");
    strcat(v_consol,argc[2]);
    strcat(v_consol, " count=1");
    printf("Reading a file using dd:\n\n");
    system(v_consol);
    return 0;
}
