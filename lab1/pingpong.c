#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    
    int p2c[2];  // parent to child
    int c2p[2];  // child to parent
    pipe(p2c);
    pipe(c2p);

    int gpid = fork();

    if(gpid > 0){
        char buf;
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], "t", sizeof buf);
        int n = read(c2p[0], &buf, sizeof buf);
        if(n>0){
            printf("%d: received pong\n", getpid());
        }
        close(p2c[1]);
        close(c2p[0]);
    } 
    else if(gpid == 0){
        char buf;
        close(c2p[0]);
        close(p2c[1]);
        int n = read(p2c[0], &buf, sizeof buf);
        if(n>0){
            printf("%d: received ping\n", getpid());
        }
        write(c2p[1], "r", sizeof buf);
        close(c2p[1]);
        close(p2c[0]);
        exit(0);
    }
    else{
        printf("Error in fork()\n");
    }
    wait(0);
    exit(0);
}