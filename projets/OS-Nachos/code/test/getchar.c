#include "syscall.h"

int main()
{
    int c;
    c = GetChar();
    if (c != -1)
    {
        PutChar((char)c);
        PutChar('\n');
    }
    else
    {
        PutString("EOF detected\n");
    }
    Halt();
}
