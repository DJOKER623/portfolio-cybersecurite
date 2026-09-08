#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg)
{
    (void)arg;
    readAvail->V();
}
static void WriteDoneHandler(void *arg)
{
    (void)arg;
    writeDone->V();
}

ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, NULL);
    lockPut = new Lock("console put lock");
    lockGet = new Lock("console get lock");
}

ConsoleDriver::~ConsoleDriver()
{
    delete console;
    delete writeDone;
    delete readAvail;
    delete lockPut;
    delete lockGet;
}

void ConsoleDriver::PutChar(int ch)
{
    lockPut->Acquire();
    console->TX(ch);
    writeDone->P();
    lockPut->Release();
}

int ConsoleDriver::GetChar()
{
    lockGet->Acquire();
    readAvail->P();
    int c = console->RX();
    lockGet->Release();
    return c;
}

void ConsoleDriver::PutString(const char *s)
{

    for (int i = 0; s[i] != '\0'; i++)
    {
        PutChar(s[i]);
    }
}

void ConsoleDriver::GetString(char *s, int n)
{

    if (n < 0)
    {
        Exit(EXIT_FAILURE);
    }
    int i = 0;
    int c;
    while (i < n - 1)
    {
        c = GetChar();
        if (c == EOF)
        {
            break;
        }
        s[i++] = c;
        if (c == '\n' || c == '\0')
        {
            break;
        }
    }
    s[i] = '\0';

    if (c != '\n' && c != EOF)
    {
        while (true)
        {
            c = GetChar();
            if (c == '\n' || c == EOF)
            {
                break;
            }
        }
    }
}

unsigned ConsoleDriver::copyStringFromMachine(int from, char *to, unsigned size)
{
    unsigned i = 0;
    int value;
    bool end = true;
    while (i < size - 1 && end)
    {
        end = machine->ReadMem(from + i, 1, &value, TRUE);
        if (end == false)
        {
            ASSERT_MSG(!(end), "Error while reading");
            break;
        }
        to[i] = (char)value;
        if (to[i] == '\0')
        {
            return i + 1;
        }
        i++;
    }
    to[i] = '\0';
    return i;
}

unsigned ConsoleDriver::copyStringToMachine(int to, const char *from, unsigned size)
{
    unsigned i = 0;
    bool ok = true;
    while (i < size - 1 && from[i] != '\0' && ok)
    {
        ok = machine->WriteMem(to + i, 1, from[i]);
        if (!ok)
        {
            ASSERT_MSG(!(ok), "Error while writing");
            break;
        }
        i++;
    }
    machine->WriteMem(to + i, 1, '\0');
    return i;
}

void ConsoleDriver::PutInt(int n)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", n);

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        lockPut->Acquire();
        console->TX(buffer[i]);
        writeDone->P();
        lockPut->Release();
    }
}

void ConsoleDriver::GetInt(int *n)
{
    char buffer[MAX_STRING_SIZE];
    int i = 0;
    int c;

    while (i < MAX_STRING_SIZE - 1)
    {
        c = GetChar();
        if (c == EOF || c == '\n')
        {
            break;
        }
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';
    *n = 0;
    sscanf(buffer, "%d", n);
}

#endif // CHANGED