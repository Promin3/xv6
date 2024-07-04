#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void generate_nums(int nums[]);
void send_info(int pd[], int info[],int infolen);
void process(int pd[]);

int
main(int argc, char *argv[])
{
    int nums[34];
    int pd[2];
    pipe(pd);

    if (fork() == 0)
    {
        process(pd);
        exit(0);
    }
    else
    {
        generate_nums(nums);
        send_info(pd, nums, 34);
        sleep(10);
        exit(0);
    }
}

void process(int pd[]){
    int info[34];
    int infolen = 0;
    int p;

    close(pd[1]);
    int len = read(pd[0], &p, sizeof p);
    printf("prime %d\n", p);

    while (len != 0)
    {
        int tmp;
        len = read(pd[0], &tmp, sizeof tmp);
        if(tmp % p != 0){
            info[infolen] = tmp;
            infolen++;
        }
    }
    close(pd[0]);

    if(infolen == 0){
        exit(0);
    }

    int child_pd[2];
    pipe(child_pd);
    if(fork() == 0){
        process(child_pd);
        exit(0);
    }else{
        send_info(child_pd, info, infolen);
        exit(0);
    }
}

void generate_nums(int nums[]){
    int x = 2;
    for (int i = 0; i < 34; i++, x++)
    {
        nums[i] = x;
    }
}

void send_info(int pd[], int info[],int infolen){
    close(pd[0]);
    for (int i = 0; i < infolen; i++)
    {
        int n = info[i];
        write(pd[1], &n, sizeof n);
    }
    close(pd[1]);
}