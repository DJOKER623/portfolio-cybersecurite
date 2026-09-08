#include "system.h"
#include "syscall.h"

struct UserThreadParams
{
    int f;
    int arg;
    int stackTop;
    int exitAddr;
};

static void StartUserThread(void *schmurtz)
{
    UserThreadParams *p = (UserThreadParams *)schmurtz;

    int f = p->f;
    int arg = p->arg;
    int stackTop = p->stackTop;
    int exitAddr = p->exitAddr;
    delete p;

    for (int i = 0; i < NumTotalRegs; i++){
        machine->WriteRegister(i, 0);
    }

    machine->WriteRegister(4, arg);
    machine->WriteRegister(31, exitAddr);
    machine->WriteRegister(StackReg, stackTop - 16);
    machine->WriteRegister(PCReg, f);
    machine->WriteRegister(NextPCReg, f + 4);
    machine->Run();
    ASSERT(false);
}

int do_ThreadCreate(int f, int arg, int exitAddr)
{
    if (f == 0)
    {
        DEBUG('s', "Error with the function\n");
        return -1;
    }
    Thread *new_thread = new Thread("forked thread");
    if (new_thread == nullptr)
    {
        DEBUG('s', "Problem with Initialization");
        return -1;
    }
    new_thread->space = currentThread->space;
    if (new_thread->space == nullptr)
    {
        DEBUG('s', "Wrong affectation");
        delete (new_thread);
        return -1;
    }
    int stackTop = currentThread->space->AllocateUserStack();
    if (stackTop == 0)
    {
        DEBUG('t', "do_ThreadCreate: plus de pile disponible.\n");
        delete new_thread;
        return -1;
    }

    new_thread->userStackTop = stackTop;
    currentThread->space->IncThreadCount();

    UserThreadParams *params = new UserThreadParams;
    params->f = f;
    params->arg = arg;
    params->stackTop = stackTop;
    params->exitAddr = exitAddr;

    new_thread->Start(StartUserThread, (void *)params);

    return 0;
}

void do_ThreadExit(void)
{
    DEBUG('t', "ThreadExit: terminaison du thread %s\n", currentThread->getName());

    if (currentThread->space != NULL)
    {
        currentThread->space->FreeUserStack(currentThread->userStackTop);
        int remaining = currentThread->space->DecThreadCount();
        if (remaining == 0)
        {
            DEBUG('t', "Dernier thread, extinction de Nachos.\n");
            interrupt->Powerdown();
        }
    }

    currentThread->Finish();
    ASSERT(false);
}