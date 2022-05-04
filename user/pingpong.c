// pingpong.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // 创建管道会得到一个长度为 2 的 int 数组
    // 其中 0 为用于从管道读取数据的文件描述符，1 为用于向管道写入数据的文件描述符
    int parent[2], child[2];
    pipe(parent);
    pipe(child);

    int pid;
    char buf[1];
    if ((pid = fork()) < 0) {
        fprintf(2, "fork error\n");
        exit(1);
    } else if (pid == 0) {  // 子进程
        read(parent[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(child[1], buf, 1);
        exit(0);
    }
    // 父进程
    write(parent[1], "!", 1);
    read(child[0], buf, 1);
    printf("%d: received pong\n", getpid());
    exit(0);

    return 0;
}