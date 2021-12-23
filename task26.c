#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100


int main (int argc, char *argv[]){

    int status;
    char buf[BUF_SIZE];
    FILE *pipe = popen("echo hello world", "r");

    if(pipe == NULL){
        if(errno == EINVAL){
            printf("\n Usage:   popen(command, 'r')\n");
            return -1;
        }else{
            printf("Error with pipe opening/n");
            return -1;
        }
    }

    unsigned int bytes_read = fread(buf, 1, BUF_SIZE, pipe);//считывает max BUF$
    if (bytes_read == -1){
        printf("Error at fread");
        return -2;
    }

    unsigned int i = 0;
    while(i < bytes_read){
        buf[i] = toupper(buf[i]);
        i++;
    }

    status = pclose(pipe);
    if(WIFEXITED(status)){
        printf("\nChild exit status: %ld\n", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }else if(WIFSIGNALED(status)){
        printf("\nChild was termonated by a signal number: %ld\n", WTERMSIG(sta$
        return  WTERMSIG(status);
    }else {
        printf("Error while pipe closing");
        return -1;
    }
                                                                            
    if(fwrite(buf, bytes_read, 1, stdout) == -1){
        printf("Error in fwrite()");
        return -3;
    }
    return 0;
}
