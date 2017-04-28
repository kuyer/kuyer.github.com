#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFERSIZE 4096

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("useage: copy srcfile dstfile\n");
        return 1;
    }

    printf("argv[0]=%s\n", argv[0]);
    printf("argv[1]=%s\n", argv[1]);
    printf("argv[2]=%s\n", argv[2]);

    printf("cp start.\n");

    // 用只读的方式打开一个已存在的文件
    int srcfd = open(argv[1], O_RDONLY);
    if(srcfd == -1) {
        printf("open %s fail.\n", argv[1]);
    }

    //创建一个新的文件，这个文件只能写，权限是0666
    int dstfd = open(argv[2], O_CREAT | O_WRONLY, 0666);
    if(dstfd == -1) {
        printf("open %s fail.\n", argv[2]);
        return 1;
    }

    int len = 0;
    char buffer[BUFFERSIZE] = {0}; //临时缓冲区

    while((len=read(srcfd, buffer, BUFFERSIZE))>0) {//循环读取数据
        // 写入文件
        if(write(dstfd, buffer, len) != len ) {
            printf("write error.\n");
            return 1;
        }
    }
    if(len < 0) {
        perror("read error.\n");
        return 1;
    }
    // 关闭文件
    close(srcfd);
    close(dstfd);

    printf("cp finish.\n");
    return 0;
}