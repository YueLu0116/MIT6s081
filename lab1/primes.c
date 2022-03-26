#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ_END 0
#define WRITE_END 1

void processNum(int p2c[]);

int main(int argc, char* argv[]){
    
    int p2c[2];
    pipe(p2c);

    int root_pid = fork();
    if(root_pid > 0){
        close(p2c[READ_END]);
        for(int num=2;num<=35;num++){
            write(p2c[WRITE_END], &num, sizeof(num));
        }
        close(p2c[WRITE_END]);
    }
    else if(root_pid == 0){
        // printf("in root process, generate a child\n");
        processNum(p2c);
    }
    wait(0);
    exit(0);
}

void processNum(int p2c[]){
    int p = 0;
    close(p2c[WRITE_END]);
    int n = read(p2c[READ_END], &p, sizeof(p));
    
    if(n>0){
        printf("prime %d\n", p);
    }
    else if(n == 0){
        close(p2c[READ_END]);
        return;
    }
    else{
        printf("Error in read() of pid: %d\n", getpid());
        return;
    }
    int new_pipe[2];
    pipe(new_pipe);
    int pid = fork();

    if(pid>0){
        close(new_pipe[READ_END]);
        int num = 0;
        while(read(p2c[READ_END], &num, sizeof(num))){
            if(num%p != 0){
                write(new_pipe[WRITE_END], &num, sizeof(num));
            }
        }
        close(new_pipe[WRITE_END]);
    }
    else if(pid == 0){
        processNum(new_pipe);
    }
    wait(0);
}