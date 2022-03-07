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
#include "limits.h"
#include "machine.h"
#include <ctime>

// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int virtAddr, int limit)
{
	int i; //chi so index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;

	memset(kernelBuf, 0, limit + 1);

	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}


// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

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

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    // if ((which == SyscallException) && (type == SC_Halt)) {
	// DEBUG('a', "Shutdown, initiated by user program.\n");
   	// interrupt->Halt();
    // } else {
	// printf("Unexpected user mode exception %d %d\n", which, type);
	// ASSERT(FALSE);
    // }
	switch (which) {
		case NoException:
			return;

		case PageFaultException:
			DEBUG('a', "\n No valid translation found");
			printf("\n\n No valid translation found");
			interrupt->Halt();
			break;

		case ReadOnlyException:
			DEBUG('a', "\n Write attempted to page marked read-only");
			printf("\n\n Write attempted to page marked read-only");
			interrupt->Halt();
			break;

		case BusErrorException:
			DEBUG('a', "\n Translation resulted invalid physical address");
			printf("\n\n Translation resulted invalid physical address");
			interrupt->Halt();
			break;

		case AddressErrorException:
			DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
			printf("\n\n Unaligned reference or one that was beyond the end of the address space");
			interrupt->Halt();
			break;

		case OverflowException:
			DEBUG('a', "\nInteger overflow in add or sub.");
			printf("\n\n Integer overflow in add or sub.");
			interrupt->Halt();
			break;

		case IllegalInstrException:
			DEBUG('a', "\n Unimplemented or reserved instr.");
			printf("\n\n Unimplemented or reserved instr.");
			interrupt->Halt();
			break;

		case NumExceptionTypes:
			DEBUG('a', "\n n exception types");
			printf("\n\n n exception types");
			interrupt->Halt();
			break;


		case SyscallException:
			switch (type) {
				case SC_Halt: 
					DEBUG('a', "\nShutdown, initiated by user program. ");
					printf("\nShutdown, initiated by user program. ");
					interrupt->Halt();
					return;

				case SC_ReadNum: {
					//Read num(int)
					char* buffer;
					int MAX_BUFFER = 255;
					buffer = new char[MAX_BUFFER + 1];
					int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);// doc buffer toi da MAX_BUFFER ki tu, tra ve so ki tu doc dc
					long long n = 0; // so luu ket qua tra ve cuoi cung

					// Xac dinh so am hay so duong (1 la am, 0 la duong)
					bool sign = 0;
					int firstIndex = 0;
					int lastIndex = 0;
					if (buffer[0] == '-') {
						sign = 1;
						firstIndex = 1;
						lastIndex = 1;
					}

					// Kiem tra so nguyen buffer
					for (int i = firstIndex; i < numbytes; i++) {
						if (buffer[i] == '.') {// 1.00
							for (int j = i + 1; j < numbytes; j++) {
								// So khong hop le
								if (buffer[j] != '0') {
									printf(" - The integer n is not valid");
									DEBUG('a', " - The integer n is not valid");
									machine->WriteRegister(2, 0);
									IncreasePC();
									delete buffer;
									return;
								}
							}
							// la so thoa cap nhat lastIndex
							lastIndex = i - 1;
							break;
						}
						else if (buffer[i] < '0' || buffer[i] > '9') {
							printf(" - The integer n is not valid");
							DEBUG('a', " - The integer n is not valid");
							machine->WriteRegister(2, 0);
							IncreasePC();
							delete buffer;
							return;
						}
						lastIndex = i;
					}

					// La so nguyen hop le, tien hanh chuyen chuoi ve so nguyen
					for (int i = firstIndex; i<= lastIndex; i++) {
						n = n * 10 + (int)(buffer[i] - 48);
					}

					// neu la so am thi * -1;
					if (sign) {
						n = n * -1;
					}
					if (n > INT_MAX) {
						n = 0;
						printf(" - Overflow");
						DEBUG('a', " - Overflow");
						machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;
					}
					else if (n < INT_MIN + 1){
						n = 0;
						printf(" - Overflow");
						DEBUG('a', " - Overflow");
						machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;
					}
					machine->WriteRegister(2, int(n));
					IncreasePC();
					delete buffer;
					return;
				}
				case SC_PrintNum: {
					//Print num(int)
					int n = machine->ReadRegister(4);
					if (n == 0) {
						gSynchConsole->Write("0", 1); // In ra man hinh so 0
						IncreasePC();
						return;
					}

					/*Qua trinh chuyen so thanh chuoi de in ra man hinh*/
					bool sign = 0; // gia su la so duong
					int nOfNum = 0; // so chu so cua n
					int firstIndex = 0;

					if (n < 0) {
						sign = 1;
						n = n * -1; // Nham chuyen so am thanh so duong de tinh so chu so
						firstIndex = 1;
					}

					int temp = n; // dem so chu so cua n
					while (temp) {
						nOfNum++;
						temp /= 10;
					}

					// Tao buffer chuoi de in ra man hinh
					char* buffer;
					int MAX_BUFFER = 255;
					buffer = new char[MAX_BUFFER + 1];
					for (int i = firstIndex + nOfNum - 1; i >= firstIndex; i--) {
						buffer[i] = (char)((n % 10) + 48);
						n /= 10;
					}
					if (sign) {
						buffer[0] = '-';
						buffer[nOfNum + 1] = 0;
						gSynchConsole->Write(buffer, nOfNum + 1);
					}
					else {
						buffer[nOfNum] = 0;
						gSynchConsole->Write(buffer, nOfNum);
					}
					delete buffer;
					IncreasePC();
					return;
				}
				case SC_RandomNum: {
					DEBUG('a', "\n");
					printf("\n\n");
					RandomInit(time(0));
					machine->WriteRegister(2, Random());
					IncreasePC();
					return;
				}
				case SC_ReadChar: {
					//Readchar
					DEBUG('a', "\n");
					printf("\n\n");
					int maxBytes = 255;
					char* buffer = new char[255];
					int numBytes = gSynchConsole->Read(buffer, maxBytes);

					if (numBytes > 1) {//Neu nhap nhieu hon 1 ky tu thi khong hop le
						printf("It is not a character");
						DEBUG('a', "\nERROR: It is not a character");
						machine->WriteRegister(2, 0);
					}
					else if (numBytes == 0) {//Ky tu rong
						printf("Empty!");
						DEBUG('a', "\nERROR: Empty!");
						machine->WriteRegister(2, 0);
					}
					else {
						//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
						char c = buffer[0];
						machine->WriteRegister(2, c);
					}
					delete buffer;
					//IncreasePC(); // error system
					//return;
					break;
				}
				case SC_PrintChar:{
           			char c = (char)machine->ReadRegister(4); // read the character from r4
					gSynchConsole->Write(&c, 1);            
            		IncreasePC();
            		return;
        		}
				default:
					break;
			}
			case SC_ReadString:
			{
				// Input: Buffer(char*), do dai toi da cua chuoi nhap vao(int)
				// Output: Khong co
				// Cong dung: Doc vao mot chuoi voi tham so la buffer va do dai toi da
				int virtAddr, length;
				char* buffer;
				virtAddr = machine->ReadRegister(4); // Lay dia chi tham so buffer truyen vao tu thanh ghi so 4
				length = machine->ReadRegister(5); // Lay do dai toi da cua chuoi nhap vao tu thanh ghi so 5
				buffer = User2System(virtAddr, length); // Copy chuoi tu vung nho User Space sang System Space
				gSynchConsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
				System2User(virtAddr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
				delete buffer; 
				IncreasePC(); // Tang Program Counter 
				return;
				//break;
			}
			case SC_PrintString:
			{
				// Input: Buffer(char*)
				// Output: Chuoi doc duoc tu buffer(char*)
				// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh
				int virtAddr;
				char* buffer;
				virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
				buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
				int length = 0;
				while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
				gSynchConsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi
				delete buffer; 
				//IncreasePC(); // Tang Program Counter 
				//return;
				break;
			}
			default:
				break;
			}
			IncreasePC();
		IncreasePC();
	}
}
