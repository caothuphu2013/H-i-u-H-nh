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
#include "string.h"
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

				case SC_Exit:
					break;
				case SC_Exec:
					break;
				case SC_Join:
					break;
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

				case SC_ReadInt: {
					char *str = new char[30];
					gsc->Read(str,30);
					int num = 0;
					int mult = 1;
											
					while (('-' == (*str)) || ((*str) == '+') || ((*str) == ' '))
					{
						if (*str == '-')
							mult = mult * -1;
						str++;
					}
					while ((*str >= '0') && (*str <= '9'))
					{
						num = (num * 10) + ((*str) - '0');
						str++;
					}
			
					int result = num * mult;
					machine->WriteRegister(2,result);
		
					break;
				}

				case SC_PrintInt: {
					int n = machine->ReadRegister(4);
					
					// Convert Number To Char*
					const int minInt = -2147483648;
					char* buff = new char[255];
					int lengthStr;
					if (n == 0) {
						buff = "0";
						lengthStr = 1;	//Nếu không cập nhật lại length thì sẽ bị lỗi 
					}
					else if (n <= minInt) { //Trường hợp min của kiểu long int	
						buff = "-2147483648";
						lengthStr = 11;	//Nếu không cập nhật lại length thì sẽ bị lỗi
					}
					else {
						int index = 0;
						int i = 0, temp = 0;
						if (n >= 0) {
							temp = n;
						}
						else {
							temp = n * -1;
							i = 1;
							index = 1;
						}

						// Xử lý chuyển đổi số dương
						while (temp != 0) {
							buff[i] = temp % 10 + 48;
							temp /= 10;
							i++;
						}
						buff[i] = '\0';

						lengthStr = i;
						int len = 0, iIndex = 0;
						if (index) {
							len = (float)lengthStr / 2.0 + 0.5;
							iIndex = 0;
						}
						else {
							len = lengthStr / 2;
							iIndex = 1;
						}
						i = index;

						while (i < len) {
							char tmp = buff[lengthStr - i - iIndex];
							buff[lengthStr - i - iIndex] = buff[i];
							buff[i] = tmp;
							i++;
						}
						buff[lengthStr] = '\0';
						if (index)
							buff[0] = '-';
					}

					gsc->Write(buff,lengthStr);
					break;
					
				}

				case SC_ReadChar: {
					char* str = new char[2];
					gsc->Read(str,2);
					
					char c = str[0];
					machine->WriteRegister(2,c);
					break;
				}
				
				case SC_PrintChar: {
					char ch = machine->ReadRegister(4);
					gsc->Write(&ch, 1);
					break;
				}

				case SC_ReadString: {
					//Đọc địa chỉ kí tự đầu tiên của chuỗi
					int address = machine->ReadRegister(4);
					//Đọc số lượng kí tự tối đa
					int limit = machine->ReadRegister(5);
					char* buff = new char[limit + 1];
					if (buff == 0)
						break;
					memset(buff,0,limit + 1);
					//Đọc từ console (khai báo gsc trong system.h và makefile.common)
					gsc->Read(buff,limit);
					System2User(address,limit,buff);
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
					//Ghi xuống console (khai báo gsc trong system.h và makefile.common)
					gsc->Write(str,len);
					if (str != NULL)
						delete[] str;
					break;
				}

				case SC_CreateFile:{
					int virtAddr;
					char* filename;
					gsc->Write("\nStart Create", 13);
					//DEBUG(‘a’,"\n SC_Create call ...");
					//DEBUG(‘a’,"\n Reading virtual address of filename");
					// Lấy tham số tên tập tin từ thanh ghi r4
					virtAddr = machine->ReadRegister(4);
					//DEBUG (‘a’,"\n Reading filename.");
					// MaxFileLength là = 32
					filename = User2System(virtAddr,255);
					if (filename == NULL)
					{
						gsc->Write("\nNot enough memory in system", 28);
						//DEBUG(‘a’,"\n Not enough memory in system");
						machine->WriteRegister(2,-1); // trả về lỗi cho chương trình người dùng
						delete filename;
						return;
					}
					//DEBUG(‘a’,"\n File name : '"<<filename<<"'");
					// Create file with size = 0
					// Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
					// việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
					// hành Linux, chúng ta không quản ly trực tiếp các block trên
					// đĩa cứng cấp phát cho file, việc quản ly các block của file
					// trên ổ đĩa là một đồ án khác
					if (!fileSystem->Create(filename,0))
					{
						gsc->Write("\nError create file",18);
						machine->WriteRegister(2,-1);
						delete filename;
						return;
					}
					machine->WriteRegister(2,0); // trả về cho chương trình
					gsc->Write("\nCreated file\n",14);
					// người dùng thành công					
				
					break;
				}
				case SC_OpenFile:{
					int indexOpen = -1;
					
					// if(fileSystem->index >= 10)
					// {
					// 	gsc->Write("\nError Open file",18);
					// 	machine->WriteRegister(2,-1);
					// 	break;
					// }
					int buffAddr = machine->ReadRegister(4);
					int type = machine->ReadRegister(5);
					if(type != 1 && type != 0)
					{
						gsc->Write("\n Error type",12);
						machine->WriteRegister(2,-1);
					
						break;
					}
					char *buf = new char[255];
					buf = User2System(buffAddr,255);
					if(strcmp(buf,"stdin") == 0)
					{
						gsc->Write("\nstdin mode\n",12);
						machine->WriteRegister(2,0);
						delete []buf;
						buf = NULL;
						break;
					}
					if(strcmp(buf,"stdout") == 0)
					{
						gsc->Write("\nstdout mode\n",13);
						machine->WriteRegister(2,1);
						delete []buf;
						buf = NULL;
						break;
					}
					for(int i = 2; i < 10; i++)
					{
						if(fileSystem->_openFile[i] == NULL)
						{
							fileSystem->_openFile[i] = fileSystem->Open_Type(buf, type);
							indexOpen = i;
							break;
						}
					}
					//printf("Mo file voi id = %d \n", indexOpen);
					if(indexOpen == -1)
					{
						gsc->Write("\nError Open file\n",19);
					 	machine->WriteRegister(2,-1);
					 	delete []buf;
						buf = NULL;
					 	break;
					}
					if(fileSystem->_openFile[indexOpen] == NULL )
					{
						gsc->Write("\nCan't open file or File not exists",36);
						machine->WriteRegister(2,-1);
					}
					else
					{
						gsc->Write("\nOpen file successfully", 24);
						machine->WriteRegister(2, indexOpen);	
					}
					delete []buf;
					buf = NULL;
					break;
				}
				case SC_CloseFile:{
					
					//printf("\nDang dong file\n");
					int openfileid = machine->ReadRegister(4);
					if(openfileid == -1)
					{
						machine->WriteRegister(2,-1);
						gsc->Write("Loi dong file\n", 14);
						break;
					}
					if(fileSystem->_openFile[openfileid] == NULL)
					{
						machine->WriteRegister(2,-1);
						gsc->Write("Loi dong file\n", 14);
						break;
					}

					delete fileSystem->_openFile[openfileid];
					fileSystem->_openFile[openfileid] = NULL;

					machine->WriteRegister(2,1);
					gsc->Write("\nDong file thanh cong\n", 22);
					break;
				}
				
				case SC_ReadFile:{
					//printf("Vo doc file");
					//char cContent[255];
					int charcount = machine->ReadRegister(5);
					int openid = machine->ReadRegister(6);
					int addrContent = machine->ReadRegister(4);
					char cContent[charcount + 1];
					if(openid < 0 || openid >= 10 || openid == 1) // 1 = console output
					{
						gsc->Write("\nopenfileid truyen vao loi\n", 27);
						machine->WriteRegister(2, -1);
						//delete[] cContent;
						//cContent = NULL;
						break;
					}
					
					if(fileSystem->_openFile[openid] == NULL)
					{
						gsc->Write("\nkhong the doc file\n", 20);
						machine->WriteRegister(2, -1);
						//delete[] cContent;
						//cContent = NULL;
						break;
					}
					if(openid == 0) //console input
					{
						int len = gsc->Read(cContent, charcount);
						len = System2User(addrContent, len, cContent);
						if(len  == 0)
						{
							gsc->Write("\nConsole cham toi cuoi file\n", 28);
							machine->WriteRegister(2, -2);
							//delete[] cContent;
							//cContent = NULL;
							break;
							
						}
						machine->WriteRegister(2, len);
						//delete[] cContent;
						//cContent = NULL;
						break;
						//scanf("%s",cContent);
					}
					else
					{
						//gsc->Write("\nVo doc file thanh cong\n", 24);
						int len  = 0;
						for(int i = 0; i < charcount; i++)
						{
			
							int sz = 0;
							sz = fileSystem->_openFile[openid]->Read(cContent + i, 1);
							if(i == 0 && sz == 0)
								break;			
							if(cContent[i] == '\0' || sz == 0)
							{								
								break;
							}
							len  += sz;
						}
						//int len = fileSystem->_openFile[openid]->Read(cContent, charcount);
						//cContent[len] = '\0';
						//len -= 1;
						if(len <= 0)
						{
							gsc->Write("\nFile rong\n", 11);
							machine->WriteRegister(2, -1);
							//delete[] cContent;
							//cContent = NULL;
							break;
							
						}
						else
						{
							//printf("\ncontent: %s", cContent);
							// printf("\nDoc file thanh cong, chieu dai %d\n",len);
							gsc->Write("\nDoc file thanh cong\n", 21);						
							len = System2User(addrContent, len, cContent);
							machine->WriteRegister(2, len);
							//delete[] cContent;
							//cContent = NULL;
							break;
							//gsc->Write("\nDoc thanh cong, chieu dai\n", 15);
							
						}
					}
					//delete[] cContent;
					//cContent = NULL;
				
					break;
				}
				case SC_WriteFile:{
					int charcount = machine->ReadRegister(5);
					int openid = machine->ReadRegister(6);
					int addrContent = machine->ReadRegister(4);
					char* content = User2System(addrContent, charcount);
					//printf("\n%s\n", content);
					if(content == NULL)
					{
						gsc->Write("\nNoi dung ghi vao file bi rong\n", 31);
						machine->WriteRegister(2, -1);
					}
					if(openid < 0 || openid >= 10 || openid == 0) // 0 = console input
					{
						gsc->Write("\nopenfileid truyen vao loi\n", 27);
						machine->WriteRegister(2, -1);
					}
					
					if(fileSystem->_openFile[openid]->_type != 0)
					{
						gsc->Write("\nchi co the doc file\n", 20);
						machine->WriteRegister(2, -1);
					}
					if(openid == 1)//console output
					{
						int len = gsc->Write(content,charcount);
						machine->WriteRegister(2, len);
					}
					else
					{
						if(fileSystem->_openFile[openid] == NULL)
						{
							gsc->Write("\nkhong the ghi file\n", 19);
							machine->WriteRegister(2, -1);
							break;
						}
						for(int i = 0; i < charcount; i++)
						{
							if(content[i] == NULL)
							{
								charcount = i;
								break;
							}
						}
						
						int len = fileSystem->_openFile[openid]->Write(content, charcount);
						
						
						// int i;
						// for (i = 0; i < charcount && content[i] != '\0'; i++)
						// {
						// 	fileSystem->_openFile[openid]->Write(content + i, 1);
						// }
						// int len = i;
						if(len <= 0)
						{
							gsc->Write("\nloi ghi file\n", 13);
							machine->WriteRegister(2, -1);
							
						}
						else
						{
							gsc->Write("\nGhi file thanh cong\n", 21);
							machine->WriteRegister(2, len);
						}
					}
					break;
				}
				case SC_SeekFile: {
					int pos = machine->ReadRegister(4);
					int openid = machine->ReadRegister(5);
					if (openid < 0 || openid >= 10){
						gsc->Write("\nopenfileid truyen vao loi", 27);
						machine->WriteRegister(2, -1);
						break;
					}

					if (openid == 0 || openid == 1) {
						gsc->Write("\nkhong cho phep seekfile trong stdin hoac stdout",48);
						break;
					}

					if (fileSystem->_openFile[openid] == NULL)
					{
						gsc->Write("\nfile bi loi",12);
						machine->WriteRegister(2, -1);
						break;
					}
					pos = (pos == -1) ? fileSystem->_openFile[openid]->Length() : pos;
					if (pos > fileSystem->_openFile[openid]->Length() || pos < 0) {
						machine->WriteRegister(2, -1);
					} else 
					{
						fileSystem->_openFile[openid]->Seek(pos);
						machine->WriteRegister(2, pos);
					}

				}
				break;

			}
			machine->registers[PrevPCReg] = machine->registers[PCReg];
			machine->registers[PCReg] = machine->registers[NextPCReg];
			machine->registers[NextPCReg] += 4;
			break;
		}
		case PageFaultException: {
			gsc->Write("No valid translation found\n",25);
			interrupt->Halt();			
			break;
		}
		case ReadOnlyException: {
			gsc->Write("Write attempted to page marked 'read-only'n",43);
			interrupt->Halt();			
			break;
		}
		case BusErrorException: {
			gsc->Write("Translation resulted in an invalid physical address\n",52);
			interrupt->Halt();			
			break;

		}
		case AddressErrorException: {
			gsc->Write("Unaligned reference or one that was beyond the end of the address space\n",72);
			interrupt->Halt();			
			break;
		}
		case OverflowException: {
			gsc->Write("Integer overflow in add or sub\n",31);
			interrupt->Halt();			
			break;
		}
		case IllegalInstrException: {
			gsc->Write("Unimplemented or reserved instr\n",32);
			interrupt->Halt();			
			break;
		}
		case NumExceptionTypes: {	
			gsc->Write("Error\n",6);
			interrupt->Halt();			
			break;
		}
		default:
			gsc->Write("Unexpected user mode exception\n",31);
			ASSERT(FALSE);
	}	
}

// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space

