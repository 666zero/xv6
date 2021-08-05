#include "kernel/types.h"
#include "user.h"

int main(int argc,char* argv[]){
    
    int parent_fd[2],child_fd[2];
    pipe(parent_fd);
    pipe(child_fd);
    char buf[1024];

    if(fork()){
        // parent process
        // printf("%d: received %s\n", getpid(), buf);
        //printf("%d: received %s\n", getpid(), buf);
        read(child_fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        write(parent_fd[1],"ping",strlen("ping"));
    }else{
        // child process
        write(child_fd[1],"pong",strlen("pong"));
        read(parent_fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        
    }
    //这样的管道一边在写的时候，另一边属于阻塞状态
    //需要看下pipe的具体实现
    exit(0);
}