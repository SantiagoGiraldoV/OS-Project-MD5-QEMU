#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define DEVICE "/dev/md5_device"

void calculate_md5(const char *filename) {
    int fd;
    char file_content[256]; // Adjust buffer size as needed
    char read_buf[33];  // 32 chars for MD5 + 1 for null terminator
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open the test file...");
        return;
    }

    fread(file_content, sizeof(file_content), 1, file);
    fclose(file);

    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device...");
        return;
    }

    printf("Writing message to the device [%s].\n", filename);
    if (write(fd, file_content, strlen(file_content)) < 0) {
        perror("Failed to write the message to the device.");
        close(fd);
        return;
    }

    printf("Reading message from the device...\n");
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Failed to read the message from the device.");
        close(fd);
        return;
    }

    printf("The MD5 hash of file [%s] is: [%s]\n", filename, read_buf);

    close(fd);
}

int main() {
    calculate_md5("testfile1.txt");
    calculate_md5("testfile2.txt");
    calculate_md5("testfile3.txt");

    return 0;
}
