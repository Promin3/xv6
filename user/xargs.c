#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
char buf[1024];

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage: xargs command ...");
        exit(1);
    }

    char *parameter[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        parameter[i - 1] = argv[i]; // 去掉xargs
    }

    parameter[argc] = 0;

    int idx = 0;
    while (read(0, buf + idx, 1))
    {
        if (buf[idx] != '\n')
        {
            idx++;
            continue;
        }

        buf[idx] = '\0'; // 把\n变为终结符

        if (fork() == 0)
        {
            parameter[argc - 1] = buf;
            exec(parameter[0], parameter);
            exit(0);
        }
        else
        {
            wait(0);
            idx = 0;
        }
    }
    exit(0);
}
