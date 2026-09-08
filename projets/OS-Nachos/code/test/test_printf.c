#include "syscall.h"

int printf(const char *fmt, ...);

int main()
{
    printf("Hello Nachos!\n");
    printf("Valeur entiere : %d\n", 42);
    printf("Texte : %s\n", "printf fonctionne !");
    Halt();
    return 0;
}
