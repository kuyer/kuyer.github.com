#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct stat st;
    int r = stat("test.txt", &st);
    if(r) {
        perror("stat error.\n");
        return -1;
    }
    printf("st_dev=%lld\n", st.st_dev);//包含这个文件的设备ID
    printf("st_ino=%ld\n", st.st_ino);//inode编号
    printf("st_mode=%d\n", st.st_mode);//访问权限
    printf("st_nlink=%d\n", st.st_nlink);//硬链接数量
    printf("st_uid=%d\n", st.st_uid);//用户ID
    printf("st_gid=%d\n", st.st_gid);//组ID
    printf("st_size=%ld\n", st.st_size);//文件占用的字节数
    //st_blksize //文件系统块大小
    //st_blocks //文件占用了几个512字节
    printf("st_atime=%ld\n", st.st_atime);//最后访问时间
    printf("st_mtime=%ld\n", st.st_mtime);//最后修改时间
    printf("st_ctime=%ld\n", st.st_ctime);//最后状态更改时间
    return 0;
}
