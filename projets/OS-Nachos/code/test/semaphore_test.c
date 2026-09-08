#include "syscall.h"


int main()
{
    int s = SemCreate(1);
    if (s < 0)
        Exit(-1);

    PutString("Semaphore created!\n");

    SemP(s);
    PutString("Entered critical section\n");
    SemV(s);
    PutString("Left critical section\n");

    SemDestroy(s);
    PutString("Semaphore destroyed!\n");

    ThreadExit();
    return 0;
}


/* EXECUTE LE P ET LE C A L'INFINI*/

/*
int semFull, semEmpty, semMutex;
int buffer = 0;

void producer(void *arg) {
    while (1) {
        SemP(semEmpty);
        SemP(semMutex);
        buffer++;
        PutChar('P');
        SemV(semMutex);
        SemV(semFull);
    }
    ThreadExit();
}

void consumer(void *arg) {
    while (1) {
        SemP(semFull);
        SemP(semMutex);
        buffer--;
        PutChar('C');
        SemV(semMutex);
        SemV(semEmpty);
    }
    ThreadExit();
}

int main() {
    semFull = SemCreate(0);
    semEmpty = SemCreate(5);
    semMutex = SemCreate(1);

    ThreadCreate(producer, 0);
    ThreadCreate(consumer, 0);

    ThreadExit();
    return 0;
}

*/