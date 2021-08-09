#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path,char* filename)
{
  
  int fd;
  struct dirent de;
  struct stat st;
  char buf[512], *p;
  //de讀取的是文件名稱
  //fd是逐步打开文件

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  //然后把读取文件的状态
  //在同一级目录下面进行扫描
  while(read(fd, &de, sizeof(de)) == sizeof(de)){

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    //把后面的部分移动到前面
    //这部分是每次修改完，然后移动走

    if (stat(buf, &st) < 0) {
			fprintf(2, "ERROR: cannot stat %s\n", buf);
		}

    switch(st.type){
    case T_FILE:
      if(strcmp(de.name, filename) == 0){
          printf("%s\n", buf);
      }
      break;
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof (buf)){
        printf("find: path too long\n");
        break;
      }
      //然后把buf放入里面
      if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
        find(buf, filename);
      }
    }
  }
  close(fd);
}
void main(int argc,char* argv[]){

    if(argc < 3){
        fprintf(2, "input parameter error\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);

    return;
}