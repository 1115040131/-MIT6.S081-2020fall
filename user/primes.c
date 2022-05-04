// primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child(int left_pipe[2]) {
    int prime;
    close(left_pipe[1]);
    // 读取第一个素数
    read(left_pipe[0], &prime, sizeof(prime));
    if (prime == 0) {  // 筛选完毕
        exit(0);
    }
    printf("prime %d\n", prime);

    int right_pipe[2];
    pipe(right_pipe);

    int pid;
    if ((pid = fork()) < 0) {
        fprintf(2, "%d fork error\n", getpid());
        exit(1);
    } else if (pid == 0) {  // 子进程
        child(right_pipe);
    }
    // 父进程
    close(right_pipe[0]);
    int n;
    while (read(left_pipe[0], &n, sizeof(n)) != 0) {
        if (n % prime != 0) {
            write(right_pipe[1], &n, sizeof(n));
        }
    }
    close(right_pipe[1]);
    wait(0);
    exit(0);
}

int main(int argc, char *argv[]) {
    int right_pipe[2];
    pipe(right_pipe);

    int pid;
    if ((pid = fork()) < 0) {
        fprintf(2, "%d fork error\n", getpid());
        exit(1);
    } else if (pid == 0) {  // 子进程
        child(right_pipe);
    }
    // 主进程，生成[2,35]的数字
    close(right_pipe[0]);
    for (int i = 2; i <= 35; ++i) {
        write(right_pipe[1], &i, sizeof(i));
    }
    close(right_pipe[1]);
    wait(0);
    exit(0);

    return 0;
}