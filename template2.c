#include <stdio.h>

#define SCSIZE 4000
char payload[SCSIZE] = ""; // Your ShellCode

char comment[712] = "";

int main(int argc, char **argv)
{
    (*(void (*)())payload)();
    return (0);
}
