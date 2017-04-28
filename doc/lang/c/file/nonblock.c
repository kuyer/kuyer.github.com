#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

char MSG_TRY[] = "try again!\n";
char MSG_TIMEOUT[] = "timeout!\n";

int main() {
    char buffer[10];
    int len;
    int fd;
    int i;

    fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);

    // 超时5秒钟后，无论有没有数据都退出
    for(i=0; i<5; i++) {
        len = read(fd, buffer, 10);
        if(len < 0) {
            if(errno == EAGAIN) {
                write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
                sleep(1);
            } else {
                perror("read error");
                exit(1);
            }
        } else {
            break;
        }
    }
    if(i == 5) {
        write(STDOUT_FILENO, MSG_TIMEOUT, strlen(MSG_TIMEOUT));
    } else {
        write(STDOUT_FILENO, buffer, len);
    }
    return 0;
}