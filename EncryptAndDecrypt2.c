#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
void encrypt(unsigned char *data, size_t len) {
    srand(1);
    for (size_t i = 0; i < len; i++) {
        data[i] = (data[i] + i) ^ (rand() % 256);
    }
    for (size_t i = 0; i < len / 2; i++) {
        size_t j = (i * 7) % len;
        unsigned char temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }
    for (size_t i = 0; i < len; i++) {
        data[i] = (data[i] * 3 + 17) % 256;
    }
    unsigned char time_key = time(NULL) % 256;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= (time_key + i);
    }
}
void decrypt(unsigned char *data, size_t len) {
    unsigned char time_key = time(NULL) % 256;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= (time_key + i);
    }
    for (size_t i = 0; i < len; i++) {
        data[i] = (data[i] - 17 + 256) % 256;
        data[i] = (data[i] * 171) % 256;
    }
    for (size_t i = len / 2; i > 0; i--) {
        size_t j = ((i - 1) * 7) % len;
        unsigned char temp = data[i - 1];
        data[i - 1] = data[j];
        data[j] = temp;
    }
    srand(1);
    for (size_t i = 0; i < len; i++) {
        data[i] ^= (rand() % 256);
        data[i] -= i;
    }
}
int main() {
    unsigned char data[] = ""; // Your ShellCode
    size_t len = strlen((char *)data);
    printf("Original Data: %s\n", data);
    encrypt(data, len);
    printf("Obfuscated Data: ");
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    decrypt(data, len);
    printf("Deobfuscated Data: %s\n", data);
    return 0;
}
