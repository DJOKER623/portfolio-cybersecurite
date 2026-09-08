#ifdef CHANGED

#include "usersemaphore.h"
#include "synch.h"
#include "system.h"

#define MAX_USER_SEMAPHORES 64

static Semaphore *userSem[MAX_USER_SEMAPHORES] = {nullptr};

int do_SemCreate(int init)
{
    if (init < 0)
        return -1;

    for (int i = 0; i < MAX_USER_SEMAPHORES; i++)
    {
        if (userSem[i] == nullptr)
        {
            userSem[i] = new Semaphore("user semaphore", init);
            return i;
        }
    }
    return -1; // plus de place disponible
}

void do_SemDestroy(int semId)
{
    if (semId < 0 || semId >= MAX_USER_SEMAPHORES)
        return;
    if (userSem[semId] == nullptr)
        return;

    delete userSem[semId];
    userSem[semId] = nullptr;
}

void do_SemP(int semId)
{
    if (semId < 0 || semId >= MAX_USER_SEMAPHORES)
        return;
    if (userSem[semId] == nullptr)
        return;

    userSem[semId]->P();
}

void do_SemV(int semId)
{
    if (semId < 0 || semId >= MAX_USER_SEMAPHORES)
        return;
    if (userSem[semId] == nullptr)
        return;

    userSem[semId]->V();
}

#endif // CHANGED
