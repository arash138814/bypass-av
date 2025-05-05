#include <stdio.h>

#define SCSIZE 4096
char payload[SCSIZE] = ""; // Your ShellCode

char comment[512] = "";

int main(int argc, char **argv)
{
    (*(void (*)())payload)();
    return (0);
}
