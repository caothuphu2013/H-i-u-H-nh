// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
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

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

/*
void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}*/

//Chuyen address to char*
char* User2System(int address,int limit) {
	char* buff = NULL;

	buff = new char[limit +1]; //Cap phat 
	if (buff == NULL)
		return buff;
	/* Ham memset duoc viet lai:
	for (int i = 0;i <= limit;i++) 
		buff[i] = 0;
	*/
	memset(buff,0,limit+1);
	int c;
	for (int i = 0 ; i < limit ;i++) {
		machine->ReadMem(address + i,1,&c);
		buff[i] = (char)c;
		if (c == 0)	//Neu gap ki tu ket thuc chuoi thi dung lai.
			break;
	}
	return buff;
}

//convert char* to 
int System2User(int address,int limit,char* buff)
{
	if (limit < 0) 
		return -1;
	if (limit == 0)
		return limit;
	int i = 0;
	int c = 0;
	do {
		c= (int) buff[i];
		machine->WriteMem(address + i,1,c);
		i++;
	}while(i < limit && c != 0);
	return i;
}

/*
//---Cau a---
void
ExceptionHandler(ExceptionType which)
{
    	int type = machine->ReadRegister(2);
	
    	if (which == NoException)
		return;

    	else if ((which == SyscallException) && (type == SC_Halt)) {
		//DEBUG('a', "Shutdown, initiated by user program.\n");
		printf("A program executed a system call %d %d\n",which,type);
   		interrupt->Halt();
    	}
	
        else if (which == PageFaultException) {
		printf("No valid translation found\n");
		interrupt->Halt();			
    	}

	else if (which == ReadOnlyException) {
		printf("Write attempted to page marked 'read-only'\n");
		interrupt->Halt();			
	}

	else if (which == BusErrorException) {
		printf("Translation resulted in an invalid physical address\n");
		interrupt->Halt();
	}

	else if (which == AddressErrorException) {
		printf("Unaligned reference or one that was beyond the end of the address space\n");
		interrupt->Halt();			
	}

	else if (which == OverflowException) {
		printf("Integer overflow in add or sub\n");
		interrupt->Halt();		
	}

	else if (which == IllegalInstrException) {
		printf("Unimplemented or reserved instr\n");
		interrupt->Halt();		
	}

	else if (which == NumExceptionTypes) {	
		printf("Error\n");
		interrupt->Halt();		
	}

        else {
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
    	}
}
*/

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
	switch (which) {
		case NoException:
			return;
		case SyscallException: {
			switch(type) {
				case SC_Halt: {
					//printf("A program executed a system call %d %d\n",which,type);
					DEBUG('a', "Shutdown, initiated by user program.\n");
					interrupt->Halt();	
					break;
				}

				case SC_Exit: {
					int exitStatus = machine->ReadRegister(4);
					int ec = ptable->ExitUpdate(exitStatus);
					machine->WriteRegister(2,ec);
					break;
				}
				case SC_Exec: {
					int address = machine->ReadRegister(4);
					char *namefile = User2System(address,255);
					if (fileSystem->Open(namefile) == NULL){
						gsc->Write("\nKhong mo duoc file",19);
						machine->WriteRegister(2,-1);
						break;
					}
					int id = ptable->ExecUpdate(namefile);
					if (id == -1) {
						machine->WriteRegister(2,-1);
					}
					else {
						machine->WriteRegister(2,id);
					}
					break;
				}
				case SC_Join: {
					int id = machine->ReadRegister(4);
					int exitcode = ptable->JoinUpdate(id);
					machine->WriteRegister(2, exitcode);
					break;
				}

				case SC_CreateSemaphore: {
					int address = machine->ReadRegister(4);
					char* namefile = User2System(address,100);
					int semval = machine->ReadRegister(5);
					int id = stable->Create(namefile,semval);
				
					if (id == -1) {
						gsc->Write("\nTao semaphore bi loi",21);
						machine->WriteRegister(2,-1);
						delete[] namefile;
						namefile = NULL;
						break;
					}
					machine->WriteRegister(2,id);
					gsc->Write("\nTao Semaphore thanh cong",25);
					delete[] namefile;
					break;	
				}
				case SC_Up: {
					int address = machine->ReadRegister(4);
					char* namefile = User2System(address,100);
					int check = stable->Signal(namefile);
					if (check == -1) { //Chưa có trong bảng stable
						gsc->Write("\nTen Semaphore khong ton tai nen khong the giai phong",53);
						machine->WriteRegister(2,-1);
						delete[] namefile;
						namefile = NULL;
						break;
					}
					machine->WriteRegister(2,check);
					delete[] namefile;
					break;
				}
				case SC_Down:{
					int address = machine->ReadRegister(4);
					char* namefile = User2System(address,100);
					int check = stable->Wait(namefile);
					if (check == -1) { //Chưa có trong bảng stable
						gsc->Write("\nTen Semaphore khong ton tai nen khong the cho doi",50);
						machine->WriteRegister(2,-1);
						delete[] namefile;
						namefile = NULL;
						break;
					}
					machine->WriteRegister(2,check);
					delete[] namefile;
					break;
				}
				case SC_Create:
					break;
				case SC_Open:
					break;
				case SC_Read:
					break;
				case SC_Write:
					break;
				case SC_Close:
					break;
				case SC_Fork:
					break;
				case SC_Yield:
					break;

				case SC_PrintChar: {
					char ch = machine->ReadRegister(4);
					gsc->Write(&ch, 1);
					break;
				}

				case SC_ReadString: {
					char *buf = new char[255];
					if (buf == 0) break;
					int bufAddrUser = machine->ReadRegister(4);		
					int length = machine->ReadRegister(5);			
					int sz = gsc->Read(buf, length + 1);	
					System2User(bufAddrUser, sz + 1, buf);	
					break;
				}

				case SC_PrintString: {
					int len = 0;
					int address = machine->ReadRegister(4);
					char* str = User2System(address,255);
					for (int i = 0;;i++) {
						len++;
						if (str[i] == 0)
							break;
					}
					gsc->Write(str,len);
					if (str != NULL)
						delete[] str;
					break;
				}

			}
			machine->registers[PrevPCReg] = machine->registers[PCReg];
			machine->registers[PCReg] = machine->registers[NextPCReg];
			machine->registers[NextPCReg] += 4;
			break;
		}
		case PageFaultException: {
			printf("No valid translation found\n");
			interrupt->Halt();			
			break;
		}
		case ReadOnlyException: {
			printf("Write attempted to page marked 'read-only'\n");
			interrupt->Halt();			
			break;
		}
		case BusErrorException: {
			printf("Translation resulted in an invalid physical address\n");
			interrupt->Halt();			
			break;

		}
		case AddressErrorException: {
			printf("Unaligned reference or one that was beyond the end of the address space\n");
			interrupt->Halt();			
			break;
		}
		case OverflowException: {
			printf("Integer overflow in add or sub\n");
			interrupt->Halt();			
			break;
		}
		case IllegalInstrException: {
			printf("Unimplemented or reserved instr\n");
			interrupt->Halt();			
			break;
		}
		case NumExceptionTypes: {	
			printf("Error\n");
			interrupt->Halt();			
			break;
		}
		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
	}	
}
