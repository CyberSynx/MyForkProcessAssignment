// C program to demonstrate use of fork() and pipe()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sigint_handler(int sig)
{
printf("Oh dear, it seems you has been interrupted");
exit(1);
}

int main(int argc, char **argv)
{

        int fd1[2]; // Used to store two ends of first pipe
        int fd2[2]; // Used to store two ends of second pipe

        char fixed_str[] = ". Good to see you";
        char input_str[100];
        pid_t p;

        if (pipe(fd1) == -1) {
                fprintf(stderr, "Pipe Failed");
                return 1;
        }
        if (pipe(fd2) == -1) {
                fprintf(stderr, "Pipe Failed");
                return 1;
        }

        if(signal(SIGINT, sigint_handler)== SIG_ERR){
        perror ("Signal");
        exit(1);
        }


        p = fork();

        if (p < 0) {
                fprintf(stderr, "fork Failed");
                return 1;
        }
        
               // Parent process
        else if (p > 0) {

                printf("Enter your name");
                scanf("%s", input_str);
                close(fd1[0]);
                char concat_str[100];

                write(fd1[1], input_str, strlen(input_str) + 1);
                close(fd1[1]);

                wait(NULL);
                close(fd1[0]);


                write(fd1[1], input_str, strlen(input_str) + 1);
                close(fd1[1]);

                wait(NULL);

                close(fd2[1]);

                read(fd2[0], concat_str, 100);
                close(fd2[0]);
        }

        // child process
        else {
                close(fd1[1]);

                char concat_str[100];
                read(fd1[0], concat_str, 100);

                int k = strlen(concat_str);
                int i;
                for (i = 0; i < strlen(fixed_str); i++)
                concat_str[k++] = fixed_str[i];
                printf("Hi %s\n", concat_str);


                concat_str[k] = '\0';
                close(fd1[0]);
                close(fd2[0]);

                write(fd2[1], concat_str, strlen(concat_str) + 1);
                close(fd2[1]);

                exit (0);
        }

}
