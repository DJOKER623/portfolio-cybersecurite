#include "syscall.h"

void printString(const char *s, int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    PutString(s);
  }
  PutChar('\n');
}

int main()
{
  printString("abcdefg", 1);
  Halt();
}
