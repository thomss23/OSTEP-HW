#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sched.h>

int main() {
    
    //==== measure a system call ====
    int fd = open("./nada.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    int loops = 1000000;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for(int i = 1; i <= loops; i++) {
        read(fd, NULL, 0);
    }

    gettimeofday(&end, NULL);

    printf("The sys call took %f microseconds\n", (float) (end.tv_usec - start.tv_usec) / loops);
    close(fd);

    //===== measure context switch ======
    int first_pipefd[2], second_pipefd[2];

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    if (pipe(first_pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(second_pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int rc = fork();

    if(rc == 0) { // child process

        if(sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) {
            exit(EXIT_FAILURE);
        }   

        for (size_t i = 0; i < loops; i++) {
            read(first_pipefd[0], NULL, 0);
            write(second_pipefd[1], NULL, 0);
        }
        

    } else {

        if(sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) {
            exit(EXIT_FAILURE);
        }

        gettimeofday(&start, NULL);

            for (size_t i = 0; i < loops; i++) {
                write(first_pipefd[0], NULL, 0);
                read(second_pipefd[1], NULL, 0);
            }
        
        gettimeofday(&end, NULL);

        printf("The context switch took %f microseconds\n", (float) (end.tv_usec - start.tv_usec) / loops);


    }

    return 0;

}