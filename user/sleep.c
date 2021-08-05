#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char* argv[])
{
    if(argc <= 1){
        printf("wrong parameter entered");
        exit(1);
    }
    int time = atoi(argv[1]);
    sleep(time);
    exit(0);
    //这里系统调用
}