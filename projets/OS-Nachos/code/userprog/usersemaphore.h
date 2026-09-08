#ifdef CHANGED
#ifndef USERSEMAPHORE_H
#define USERSEMAPHORE_H

extern int do_SemCreate(int init);
extern void do_SemDestroy(int semId);
extern void do_SemP(int semId);
extern void do_SemV(int semId);

#endif
#endif // CHANGED
