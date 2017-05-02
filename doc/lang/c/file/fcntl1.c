#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("usage: fcntl1 [fd]\n");
        exit(1);
    }
    int fd = atoi(argv[1]);
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1) {
        perror("fcntl error.\n");
        exit(1);
    }
    if(flags && O_RDONLY) {
        printf("rdonly.\n");
    }
    if(flags && O_WRONLY) {
        printf("wronly.\n");
    }
    if(flags && O_RDWR) {
        printf("rdwr.\n");
    }
    if(flags && O_NONBLOCK) {
        printf("nonblock.\n");
    }
    if(flags && O_APPEND) {
        printf("append.\n");
    }
    return 0;
}