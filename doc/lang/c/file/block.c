#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char buf[100];
    int len;
    while(1) {
        //STDIN_FILENO 是标准输入描述符，值为0
        printf("in. %s\n", buf);
        len = read(STDIN_FILENO, buf, 10);
        //STDOUT_FILENO 是标准输出描述符，值为1
        printf("out. %s\n", buf);
        write(STDOUT_FILENO, buf, len);
        printf("end. %s\n", buf);
    }
    return 0;
}