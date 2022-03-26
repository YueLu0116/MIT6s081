#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
      fprintf(2, "find: cannot open %s\n", path);
      return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }    
        switch(st.type){
        case T_FILE:
            if(strcmp(name, de.name) == 0){
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            find(buf, name);
            break;
        }
    }

    close(fd);
}

int main(int argc, char *argv[]){
    if(argc <= 2){
        printf("Usage: find <path> <name>\n");
        exit(-1);
    }
    char *path = argv[1];
    char *name = argv[2];
    find(path, name);
    exit(0);
}