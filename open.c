#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int fd; // 文件描述符
    char *filename;

    // 检查命令行参数
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];

    // 使用 open 系统调用打开文件
    // O_RDONLY 表示以只读方式打开文件
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        // 如果打开失败，打印错误信息
        perror("open");
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("File %s opened successfully. File descriptor: %d\n", filename, fd);

    // 关闭文件描述符
    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    printf("File descriptor %d closed successfully.\n", fd);

    return EXIT_SUCCESS;
}
