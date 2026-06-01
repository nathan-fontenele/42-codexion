#include "codexion.h"

long get_time_in_ms(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int main (int argc, char **argv)
{
    if (argc != 9)
    {
        printf("number of invalid arguments.\n");
        return(1);
    }
    
    int i;
    
    i = 1;
    while (i <= 7)
    {
        if (atoi(argv[i]) < 0)
        {
            printf("Error: Argument %d must be a positive interger.\n", i);
            return(1);
        }
        i++;
    }

    if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
    {
        printf("Error: Schedule must be 'fifo' or 'edf'.\n");
        return(1);
    }

    printf("Arguments validated. Starting simulation...\n");
    return(0);
}