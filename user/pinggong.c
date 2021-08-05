#include "kernel/types.h"
#include "kernel/pipe.c"
#include "kernel/user.h"

int main(int argc,char* argv[]){
    
    int parent_fd[2],child_fd[2];
    pipe(parent_fd);
    pipe(child_fd);
    char buf[1024];

    if(fork()){
        // parent process
        write(parent_fd[1],"ping",strlen("ping"));
        read(child_fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
    }else{
        // child process
        read(parent_fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        write(child_fd[1],"pong",strlen("pong"));
    }
}