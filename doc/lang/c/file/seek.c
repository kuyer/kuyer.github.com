#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if(argc <= 1) {
        printf("useage: seek file_path.\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if(fd<0) {
        printf("open %s fail.\n", argv[1]);
        return 1;
    }
    char buf[64] = {0};
    lseek(fd, 6, SEEK_SET);
    read(fd, buf, 64);
    printf("%s\n", buf);
    return 0;
}