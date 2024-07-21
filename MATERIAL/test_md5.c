#include <stdio.h>
#include <string.h>
#include "md5.c"

void print_md5(unsigned char *md) {
    for (int i = 0; i < 16; i++) {
        printf("%02x", md[i]);
    }
    printf("\n");
}

int main() {
    unsigned char digest[16];
    char *string = "Hello World";
    MD5_CTX context;

    MD5Init(&context);
    MD5Update(&context, (unsigned char *)string, strlen(string));
    MD5Final(digest, &context);

    printf("MD5 (\"%s\") = ", string);
    print_md5(digest);

    return 0;
}
