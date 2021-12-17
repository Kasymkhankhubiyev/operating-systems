#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
    int status;

    if (argc < 2){
        printf("Not enough arguments entered\n");
        return -1;
    }

    pid_t pid = fork();
    if(pid == -1){
        if(errno == EAGAIN){
            printf("fork() cannot allocate memory - the limit is reached\n");
            return -1;
        }else {
            printf("Error in fork()\n");
            return -1;
        }
    }
    if(pid == 0){
        execl("/bin/cat", "cat", argv[1], NULL);
        perror("exec failed");
        printf("\n  Parent... \n"); //не напечатается, т.к. мы уже в новом
        //процессе вызвали execl() и заменили контекст.
        return -1;
    }

    waitpid(pid, &status, 0);//WNOHANG
    if(WIFEXITED(status)){
        printf("\nChiled exit status:  %ld\n", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }else {
        if(WIFSIGNALED(status)){
            printf("signal is: %d\n");
            return -1;
        }
        if(errno = EINTR){//wait is interupted by a signal
            printf("wait was interupted by a signal\n");
            return -1;
        }else{
            printf("Error in terminating child process\n");
            return -1; //лучше проверить на ошибкиж
        }
    }
    return 0;
}


