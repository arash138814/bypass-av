#include <windows.h>
#include <stdio.h>
#include <string.h>
void xor (char *ShellCode, size_t ShellCode_len, char *key, size_t key_len) {
    for (size_t i = 0; i < ShellCode_len; i++)
    {
        ShellCode[i] ^= key[i % key_len];
    }
}
    int main()
{
    char ShellCode[] = ""; // Your ShellCode
    char key[] = "1";
    xor(ShellCode, strlen(ShellCode), key, strlen(key));
    printf("Encrypted: %s\n", ShellCode);
    xor(ShellCode, strlen(ShellCode), key, strlen(key));
    printf("Decrypted: %s\n", ShellCode);
    return 0;
}
