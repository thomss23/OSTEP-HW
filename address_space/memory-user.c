#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "usage: memory-user <memory> <time>\n");
        exit(EXIT_FAILURE);
    }

    printf("pid: %d\n", getpid());

    int megabytes = (1024 * 1024) * atoi(argv[1]);
    int lengthOfTheArray = (int) (megabytes / sizeof(int));
    int timeToRun = atoi(argv[2]);

    int *arr = (int*) malloc(lengthOfTheArray);

    time_t startTime;
    float setTime = timeToRun;

    time(&startTime);
    while ((difftime(time(NULL), startTime) < setTime)) {
        
        for(int i = 0; i < lengthOfTheArray; i++) {

            if(difftime(time(NULL), startTime) < setTime) {
                break;
            }

            arr[i] = 1;
        }
        
    }
    free(arr);

}