#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LEN 100


int main(int argc, char *argv[]) {
	char *command = argv[1];
	char buf;
	char paramv[MAXARG][MAX_LEN]; 
	char *subargs[MAXARG];

    while(1){
        int count = argc-1;  
		memset(paramv, 0, MAXARG * MAX_LEN);
		
		for (int i = 1; i < argc; i++) {
			strcpy(paramv[i - 1], argv[i]);
		}
   
		int cursor = 0; 
		int flag = 0; 
		int res;

        while(((res = read(0, &buf, 1) > 0)) && buf != '\n'){
            if(buf == ' ' && flag == 1){
                cursor = 0;
                flag = 0;
                count ++;
            }else if(buf != ' '){
                paramv[count][cursor ++] = buf;
                flag = 1;
            }
        }
        
        
        if (res <= 0) {
			break;
		}
		for (int i = 0; i < MAXARG - 1; i++) {
			subargs[i] = paramv[i];
		}
		subargs[MAXARG - 1] = 0;
		if (fork() == 0) {
			exec(command, subargs);
			exit(0);
		} else {
			wait((int *) 0);
		}

    }
    exit(0);
}