#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int p[2];
    pipe(p);
    char* msg[10];
    if(fork() == 0){
        read(p[0], msg, 10);
        printf("%d: received %s\n",getpid(),msg);
        write(p[1], "pong", 10);
        exit(0);
    }else{
        write(p[1], "ping", 10);
        wait(0);
        read(p[0],msg,10);
        printf("%d: received %s\n",getpid(),msg);
    } 
    exit(0);//父进程结束前子进程也要退出
}