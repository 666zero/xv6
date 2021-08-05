#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int index);

//这里面少了一个I/O重定向的过程
//这里有7个primes进程说明没有关闭相关的句柄
//然后查看exit系统调用发现还需要使用wait来完成
//相关的操作

void main(int argc,char* argv[]){
    
    int fd[2];
    pipe(fd);

    if(fork() == 0){
        //redirect(fd[0], fd);
        close(fd[1]);
        primes(fd[0]);
    }else{
        // parent process
        // 发送数字
        close(fd[0]);
        int i;
        for(i = 2;i < 40;i ++){
            write(fd[1], &i, sizeof(int));
            //printf("输入的%d为\n", i);
        }
        close(fd[1]);
        wait((int*)0);
        //printf("%d process wait end\n",getpid());
        exit(0);
    }
   return;
    //需要看下pipe的具体实现
}

//這裏面文件描述符有問題
//需要嘗試下面看下管道的使用
//管道是内存中一快區域

void primes(int index){

    int base;
    //printf("此时的pid为%d\n", getpid());

    if(read(index, &base, sizeof(base)) <= 0){
        //printf("读到最后一个数\n");
        exit(0);
        //子进程只需要读取，子进程如果可以读取的话
    }

    int pipe_id[2];
    printf("prime %d\n", base);

    if(pipe(pipe_id) < 0){
        fprintf(2, "pipe error\n");
        exit(0);
    }

    if(fork() == 0){
        //printf("進入了子進程\n");
        //子进程关闭写端口
        close(pipe_id[1]);
        primes(pipe_id[0]);
    }else{
        int n;
        close(pipe_id[0]);
        while(read(index, &n, sizeof(int)) > 0){
            if(n % base != 0){
                write(pipe_id[1], &n, sizeof(int));
            }
            //printf("%d n number 为\n",n);
        }
        close(index);
        close(pipe_id[1]);
        wait((int*)0);
        //printf("%d process wait end\n",getpid());
        exit(0);
    }
}
