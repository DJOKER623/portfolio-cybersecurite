#include "syscall.h"

int main()
{
    int x;

    PutString("Tapez un entier : ");
    GetInt(&x);

    PutString("Vous avez saisi : ");
    PutInt(x);
    PutChar('\n');

    Halt();
}
