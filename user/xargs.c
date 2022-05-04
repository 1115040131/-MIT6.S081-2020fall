// xargs.c
#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

int readline(int curr_argc, char *new_argv[MAXARG]) {
    char buf[1024];
    memset(buf, 0, 1024);
    int offset = 0;
    new_argv[curr_argc++] = buf;

    // 从终端读入参数
    char c;
    while (read(0, &c, 1) > 0 && c != '\n') {
        if (offset == 1023) {
            fprintf(2, "xargs: argument is too long\n");
            exit(1);
        }
        if (c == ' ') {
            buf[offset++] = '\0';
            new_argv[curr_argc++] = buf + offset;
        } else {
            buf[offset++] = c;
        }
    }

    buf[offset] = '\0';
    if (offset == 0) {
        return 0;
    }
    new_argv[curr_argc] = 0;

    return curr_argc;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "usage: xargs <command> (arg ...)\n");
        exit(1);
    }

    // 读取原有命令参数
    char *command = argv[1];
    char *new_argv[MAXARG];
    for (int i = 1; i < argc; ++i) {
        new_argv[i - 1] = argv[i];
    }

    // 若终端有参数输入，则通过子进程执行命令
    while (readline(argc - 1, new_argv) != 0) {
        if (fork() == 0) {
            exec(command, new_argv);
            fprintf(2, "%s failed\n", command);
            exit(1);
        }
        wait(0);
    }
    exit(0);
}