#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define BUF_SIZE 100

int main(int argc, char *argv[]){

    int status1, status2;
    int pipefd[2];
    char buf[BUF_SIZE];

    if(argc < 2){
        printf(" \nUsage: \n%s <text>\n", argv[0]);
        return -1;
    }

    if(pipe(pipefd)== -1){
        perror("Eror while pipe opening/n");
        return -1;
    }

    pid_t pid1 = fork();
    if(pid1 == -1){
        if(errno == EAGAIN){
            printf("the quantity of existing processes has reached it's limit\n");
            return -1;
        }else{
            printf("Error in fork()\n");
            return -1;
        }
    }

    if(pid1 == 0){
        int end_state = 0;
        if(close(pipefd[0]) == -1){//закрываем не нужный дескриптор для чтения - будем только писать!
            printf("Error at close pipefd[0]\n");
            end_state = -1;
        }
        if(write(pipefd[1], argv[1], strlen(argv[1])) != strlen(argv[1])){
            printf("Cannot write all data\n");
            if(close(pipefd[1]) == -1) // закрываем файл записи
                printf("Error at close pipefd[1]\n");
            return -1;
        }else{
            if(close(pipefd[1] == -1)){
                printf("Error at close pipefd\n");
                return -1;
            }
        }
        return end_state;
    }//else{
        pid_t pid2 = fork();
        if(pid2 == -1){
            printf("Error at fork() {pid2}\n");
            if(close(pipefd[0]) == -1)
                printf("Error at close pipefd[0] {pid2}\n");
            if(close(pipefd[1]) == -1)
                printf("Error at close pipefd[1] {pid2}\n");
            return -1;
        }
        if(pid2 == 0){
            int end_state = 0;
            if(close(pipefd[1]) == -1){//только для чтения
                printf("Error at close pipefd[1] {pid2}\n");
                end_state = -1;
            }
            //if(read(pipefd[0], buf, strlen(argv[1])) < 0){
            int bytes_read;
            if((bytes_read = read(pipefd[0], buf, BUF_SIZE)) < 0){
                printf("Cannot read string.\n");
                if(close(pipefd[0]) == -1)
                    printf("Error at close pipefd[0] {pid2}\n");
                return -1;
            }else{
                int i = 0;
                while(i < bytes_read){
                    buf[i] = (char)toupper(buf[i]);
                    i++;
                }
                printf("Upper text: %s\n", buf);
                if(close(pipefd[0]) == -1){
                    printf("Error at close pipefd[0] {pid2}\n");
                    return -1;
                }
                return end_state;
            }
        }
    //}

    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    if (close(pipefd[0]) == -1){
        printf("Error at close pipefd[0] P\n");
        return -1;
    }
                return end_state;
            }
        }
    //}

    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    if (close(pipefd[0]) == -1){
        printf("Error at close pipefd[0] P\n");
        return -1;
    }
    if(close(pipefd[1]) == -1){
        printf("Error at close pipefd[1] P\n");
        return -1;
    }
    return 0;
}
