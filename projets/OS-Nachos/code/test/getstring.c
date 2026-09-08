#include "syscall.h"

int main()
{
    char buffer[265];

    PutString("Veuillez taper une ligne (max 63 caracteres) :\n");

    GetString(buffer, 64);

    PutString("Vous avez tape : ");
    PutString(buffer);
    PutChar('\n');

    Halt();
}
