#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char whitespace[] = " \t\r\n\v";
void runcmd(char **argv, int argc);
#define MAXARGS 32
#define LEN 1024

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(-1);
}

int 
fork1() {
  int pid;
  
  pid = fork();
  if (pid < 0) {
      panic("fork");
  }
  return pid;
}

int getcmd(char* buf, int nbuf)
{
  fprintf(2, "@");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  //printf("輸入的字符為%s\n", buf);
  //返回-1，进程退出
  if(buf[0] == 0) {
    return -1;
  }
  return 0;
}

int split(char* cmd,char** argv){
    int i = 0,j = 0,len = 0;

    len = strlen(cmd);
    while(i < len && cmd[i]){
      while(i < len && strchr(whitespace, cmd[i])){
        i ++;
      }
      if(i < len){
        argv[j ++] = cmd + i;
      }
      while(i < len && !strchr(whitespace, cmd[i])){
        i ++;
      }
      cmd[i ++] = 0;
    }

    argv[j] = 0;
    return j;
}

//<是右侧的导入左侧
//
void runredir(char tok, char * file) {
  switch (tok) {
  case '<':
    close(0);   
    open(file, O_RDONLY);
    break;
  case '>':
    close(1);
    open(file, O_WRONLY|O_CREATE);
    break;  
  default:
    break;
  }
}

//0为读管道，1为写管道
void runpipe(char **argv, int argc, int index) { 
  int p[2];
  pipe(p);   

  if (fork1() == 0) {
    close(1);  
    dup(p[1]);   
    close(p[0]);      
    close(p[1]);
    runcmd(argv, index); 
    close(0);
  }else{
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    runcmd(argv + index + 1, argc - index - 1); 
    close(1);
  }
  
  //右侧不能读
  close(p[0]);
  close(p[1]);
  
  wait(0);
  wait(0);
}


void runcmd(char** argv, int argc){
  char *cmd[MAXARGS];
  int i,j = 0;
  char tok;
  for(i = 0;i < argc;i ++){
      if(strcmp(argv[i], "|") == 0){
        runpipe(argv, argc, i);
        return;
      }
  }
  for(i = 0;i < argc;i ++){
    tok = argv[i][0];
    if(strchr("<>", tok)){
      if (argv[i][1]) {
        runredir(tok, argv[i] + 1);   
      } else {
        if (i == argc-1) {        
          panic("missing file for redirection");
        }
        runredir(tok, argv[i + 1]);   
        i++;
      }
    }else{
      cmd[j ++] = argv[i];
    }
  }
  cmd[j] = 0;
  exec(cmd[0], cmd);
}

int main(void){

  int argc;
  char buf[LEN];
  char* argv[MAXARGS];

  while(getcmd(buf, sizeof(buf)) >= 0){
    if(fork1() == 0){
        argc = split(buf, argv);
        runcmd(argv, argc);
    }
    wait(0);
  }
  exit(0);
}
//这里需要看下xv6的shell实现



