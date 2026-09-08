// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"

/* --- TD2 PART 4  --- */
#include "usersemaphore.h"


//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC()
{
  int pc = machine->ReadRegister(PCReg);
  machine->WriteRegister(PrevPCReg, pc);
  pc = machine->ReadRegister(NextPCReg);
  machine->WriteRegister(PCReg, pc);
  pc += 4;
  machine->WriteRegister(NextPCReg, pc);
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
  int type = machine->ReadRegister(2);
  int address = machine->ReadRegister(BadVAddrReg);

  switch (which)
  {
  case SyscallException:
  {
    switch (type)
    {
    case SC_Halt:
    {
      DEBUG('s', "Shutdown, initiated by user program.\n");
      interrupt->Powerdown();
      break;
    }

#ifdef CHANGED
    case SC_PutChar:
    {
      DEBUG('s', "Putchar\n");
      char arg1 = (char)machine->ReadRegister(4);
      consoledriver->PutChar(arg1);
      break;
    }

    case SC_PutString:
    {
      DEBUG('s', "PutString\n");

      int string = machine->ReadRegister(4);
      char buffer[MAX_STRING_SIZE];
      int copied;

      do
      {
        copied = consoledriver->copyStringFromMachine(string, buffer, MAX_STRING_SIZE);
        buffer[MAX_STRING_SIZE - 1] = '\0';

        consoledriver->PutString(buffer);
        string += copied;

      } while (copied == MAX_STRING_SIZE - 1 && buffer[copied - 1] != '\0');

      break;
    }

    case SC_GetString:
    {
      DEBUG('s', "GetString\n");

      int string = machine->ReadRegister(4);
      int size = machine->ReadRegister(5);
      int copied_total = 0;
      char buffer[MAX_STRING_SIZE];
      int buf_size;

      while (size > 1)
      {
        buf_size = (size < MAX_STRING_SIZE) ? size : MAX_STRING_SIZE;
        consoledriver->GetString(buffer, buf_size);
        consoledriver->copyStringToMachine(string, buffer, buf_size);

        int len = strlen(buffer);
        copied_total += len;
        string += len;
        size -= len;

        if (buffer[len - 1] == '\0' || buffer[len - 1] == '\n')
          break;
      }

      break;
    }

    case SC_GetChar:
    {
      DEBUG('s', "GetChar\n");
      int c = consoledriver->GetChar();
      machine->WriteRegister(2, c);
      break;
    }

    case SC_PutInt:
    {
      DEBUG('s', "PutInt\n");
      int n = machine->ReadRegister(4);
      char buffer[32];
      snprintf(buffer, sizeof(buffer), "%d", n);
      consoledriver->PutString(buffer);
      break;
    }

    case SC_GetInt:
    {
      DEBUG('s', "GetInt\n");
      int addr = machine->ReadRegister(4);
      char buffer[MAX_STRING_SIZE];
      consoledriver->GetString(buffer, MAX_STRING_SIZE);

      int value = 0;
      sscanf(buffer, "%d", &value);

      machine->WriteMem(addr, sizeof(int), value);
      break;
    }

    case SC_Exit:
    {
      int status = machine->ReadRegister(4);
      DEBUG('s', "Exit\n");
      printf("Program exited with status %d\n", status);
      interrupt->Powerdown();
      break;
    }

    case SC_ThreadCreate:
    {
      DEBUG('s', " Create thread\n ");
      int f = machine->ReadRegister(4);
      int arg = machine->ReadRegister(5);
      int exitAdrr = machine->ReadRegister(6);
      int result = do_ThreadCreate(f, arg, exitAdrr);
      machine->WriteRegister(2, result);
      break;
    }

    case SC_ThreadExit:
    {
      DEBUG('s', " Exit thread\n ");
      do_ThreadExit();
      break;
    }

    case SC_SemCreate:
    {
      DEBUG('s', " Semaphore Create\n ");
      int init = machine->ReadRegister(4);
      int semId = do_SemCreate(init);
      machine->WriteRegister(2, semId);
      break;
    }
    case SC_SemDestroy:
    {
      DEBUG('s', " Semaphore Destroy\n ");
      int semId = machine->ReadRegister(4);
      do_SemDestroy(semId);
      break;
    }
    case SC_SemP:
    {
      DEBUG('s', " Semaphore P\n ");
      int semId = machine->ReadRegister(4);
      do_SemP(semId);
      break;
    }
    case SC_SemV:
    {
      DEBUG('s', " Semaphore V\n ");
      int semId = machine->ReadRegister(4);
      do_SemV(semId);
      break;
    }

#endif // CHANGED
    default:
    {
      ASSERT_MSG(FALSE, "Unimplemented system call %d\n", type);
    }
    }

    // Do not forget to increment the pc before returning!
    // This skips over the syscall instruction, to continue execution
    // with the rest of the program
    UpdatePC();
    break;
  }

  case PageFaultException:
    if (!address)
    {
      ASSERT_MSG(FALSE, "NULL dereference at PC %x!\n", machine->registers[PCReg]);
    }
    else
    {
      // For now
      ASSERT_MSG(FALSE, "Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
    }
    break;

  case ReadOnlyException:
    // For now
    ASSERT_MSG(FALSE, "Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
    break;

  case BusErrorException:
    // For now
    ASSERT_MSG(FALSE, "Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
    break;

  case AddressErrorException:
    // For now
    ASSERT_MSG(FALSE, "Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
    break;

  case OverflowException:
    // For now
    ASSERT_MSG(FALSE, "Overflow at PC %x\n", machine->registers[PCReg]);
    break;

  case IllegalInstrException:
    // For now
    ASSERT_MSG(FALSE, "Illegal instruction at PC %x\n", machine->registers[PCReg]);
    break;

  default:
    ASSERT_MSG(FALSE, "Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
    break;
  }
}
