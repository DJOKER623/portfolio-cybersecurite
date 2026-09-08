#include "syscall.h"

void thread(void *arg)
{
  volatile int i;
  for (i = 0; i < 2; i++)
  {
    PutChar('a');
  }
}

int main()
{
  ThreadCreate((int)thread, 0);
  ThreadCreate((int)thread, 0);
  ThreadCreate((int)thread, 0);
  return 0;
}
