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

#define MaxFileLength 32
// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

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

void ExceptionHandler(ExceptionType which)
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
					char* buffer;
					buffer = new char[256];
					int numbytes = gSynchConsole->Read(buffer, 255);// doc buffer toi da MAX_BUFFER ki tu, tra ve so ki tu doc dc
					long long n = 0; // ket qua

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
								if (buffer[j] != '0') {
									printf(" - The integer n is not valid");
									DEBUG('a', " - The integer n is not valid");
									machine->WriteRegister(2, 0);
									IncreasePC();
									delete buffer;
									return;
								}
							}
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

					for (int i = firstIndex; i<= lastIndex; i++) {
						n = n * 10 + (int)(buffer[i] - 48);
					}

					// xu ly so am
					if (sign == 1) {
						n *= (-1);
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
					int n = machine->ReadRegister(4);
					if (n == 0) {
						gSynchConsole->Write("0", 1);
						IncreasePC();
						return;
					}

					// Doi chuoi -> so
					bool sign = 0;
					int nOfNum = 0; // Do dai so
					int firstIndex = 0;

					if (n < 0) { // Kiem tra am duong
						sign = 1;
						n *= (-1);
						firstIndex = 1;
					}

					int temp = n; // Dem so chu so cua n
					while (temp) {
						nOfNum++;
						temp /= 10;
					}

					// Doi so -> chuoi
					char* buffer;
					buffer = new char[256];
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
					RandomInit(time(0));
					machine->WriteRegister(2, Random());
					IncreasePC();
					return;
				}
				case SC_ReadChar: {
					DEBUG('a', "\n");
					int maxBytes = 255;
					char* buffer = new char[255];
					int numBytes = gSynchConsole->Read(buffer, maxBytes);

					if (numBytes > 1) {
						printf("It is not a character");
						DEBUG('a', "\nERROR: It is not a character");
						machine->WriteRegister(2, 0);
					}
					else if (numBytes == 0) {
						printf("Empty!");
						DEBUG('a', "\nERROR: Empty!");
						machine->WriteRegister(2, 0);
					}
					else {
						char c = buffer[0];
						machine->WriteRegister(2, c);
					}
					delete buffer;
					break;
				}
				case SC_PrintChar:{
           			char c = (char)machine->ReadRegister(4); // Doc ki tu tu thanh ghi 4
					gSynchConsole->Write(&c, 1);
            		IncreasePC();
            		return;
        		}
				case SC_ReadString:{
					int virtAddr, length;
					char* buffer;
					virtAddr = machine->ReadRegister(4);
					length = machine->ReadRegister(5);
					buffer = User2System(virtAddr, length);
					gSynchConsole->Read(buffer, length);
					System2User(virtAddr, length, buffer);
					delete buffer;
					IncreasePC();
					return;
				}
				case SC_PrintString:{
					int virtAddr;
					char* buffer;
					virtAddr = machine->ReadRegister(4);
					buffer = User2System(virtAddr, 255);
					int length = 0;
					while (buffer[length] != 0) {
						length++;
					}
					gSynchConsole->Write(buffer, length + 1);
					delete buffer;
					IncreasePC();
					return;
				}
				case SC_Create: {
					int virtAddr;
					char* filename;
					virtAddr = machine->ReadRegister(4); 
					filename = User2System(virtAddr, MaxFileLength + 1);

					if (strlen(filename) == 0){
						printf("\n File name is not valid");
						machine->WriteRegister(2, -1); 
						IncreasePC();
						return;
					}

					if (filename == NULL){
						printf("\n Not enough memory in system");
						machine->WriteRegister(2, -1);
						delete filename;
						IncreasePC();
						return;
					}

					if (!fileSystem->Create(filename, 0))
					{
						printf("\n Error create file '%s'", filename);
						machine->WriteRegister(2, -1);
						delete filename;
						IncreasePC();
						return;
					}

					machine->WriteRegister(2, 0);
					delete filename;
					IncreasePC();
					return;
				}
				case SC_Open: {
					int virtAddr;
					int type;
					char *filename;
					int freeSlot;

					virtAddr = machine->ReadRegister(4);
					type = machine->ReadRegister(5);

					filename = User2System(virtAddr, MaxFileLength);

					freeSlot = fileSystem->FindFreeSlot();
					if (freeSlot == -1){
						printf("\nFull slot in openTable");
						machine->WriteRegister(2, -1);
						delete[] filename;
						IncreasePC();
						return;
					}

					switch (type)
					{
					case 0: //  Read and write
					case 1: //  Read only
						if ((fileSystem->table[freeSlot] = fileSystem->Open(filename, type))){
							machine->WriteRegister(2, freeSlot);
							printf("\nOpen file success!");
						}
						else
						{
							printf("\nFile does not exist");
							machine->WriteRegister(2, -1);
						}
						break;
					case 2:                            //  stdin - read from console
						machine->WriteRegister(2, 0); // stdin have tableileID 0
						printf("\nOpen file success!");
						break;
					case 3:                            //  stdout - write to console
						machine->WriteRegister(2, 1); // stdout have tableileID 1
						printf("\nOpen file success!");
						break;
					default:
						printf("\nType is not match");
						machine->WriteRegister(2, -1);
						break;
					}
					delete[] filename;
					IncreasePC();
					return;
				}
				case SC_Close: {
					int fid = machine->ReadRegister(4);
					if (fid >= 0 && fid <= 14){
						if (fileSystem->table[fid])
						{
							delete fileSystem->table[fid];
							fileSystem->table[fid] = NULL;
							machine->WriteRegister(2, 0);
							IncreasePC();
							return;
						}
					}
					machine->WriteRegister(2, -1);
					IncreasePC();
					return;
				}
				case SC_Read: {
					int virtAddr = machine->ReadRegister(4);
					int charcount = machine->ReadRegister(5);
					int id = machine->ReadRegister(6);
					int OldPos;
					int NewPos;
					char *buf;
					
					if (id < 0 || id > 14){
						printf("\nKhong the read vi id nam ngoai bang mo ta file.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}
					
					if (fileSystem->table[id] == NULL){
						printf("\nKhong the read vi file nay khong ton tai.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}

					if (fileSystem->table[id]->type == 3) // file stdout (type quy uoc la 3) thi tra ve -1
					{
						printf("\nKhong the read file stdout.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}
					OldPos = fileSystem->table[id]->GetCurrentPos();
					buf = User2System(virtAddr, charcount);
					// file stdin (type quy uoc la 2)
					if (fileSystem->table[id]->type == 2){
						int size = gSynchConsole->Read(buf, charcount);
						System2User(virtAddr, size, buf);
						machine->WriteRegister(2, size);
						delete buf;
						IncreasePC();
						return;
					}
					// file binh thuong
					if ((fileSystem->table[id]->Read(buf, charcount)) > 0){
						NewPos = fileSystem->table[id]->GetCurrentPos();
						System2User(virtAddr, NewPos - OldPos, buf);
						machine->WriteRegister(2, NewPos - OldPos);
					}
					else{
						machine->WriteRegister(2, -2);
					}
					delete buf;
					IncreasePC();
					return;
				}
				case SC_Write: {
					int virtAddr = machine->ReadRegister(4); 
					int charcount = machine->ReadRegister(5);
					int id = machine->ReadRegister(6);
					int OldPos;
					int NewPos;
					char *buf;
					if (id < 0 || id > 14){
						printf("\nKhong the write vi id nam ngoai bang mo ta file.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}

					if (fileSystem->table[id] == NULL){
						printf("\nKhong the write vi file nay khong ton tai.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}
					
					if (fileSystem->table[id]->type == 1 || fileSystem->table[id]->type == 2)
					{
						printf("\nKhong the write file stdin hoac file only read.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}
					OldPos = fileSystem->table[id]->GetCurrentPos();
					buf = User2System(virtAddr, charcount);
					// file read & write (type quy uoc la 0)
					if (fileSystem->table[id]->type == 0){
						if ((fileSystem->table[id]->Write(buf, charcount)) > 0){
							NewPos = fileSystem->table[id]->GetCurrentPos();
							machine->WriteRegister(2, NewPos - OldPos);
							delete buf;
							IncreasePC();
							return;
						}
					}
					if (fileSystem->table[id]->type == 3){  // file stdout (type quy uoc la 3)
						int i = 0;
						while (buf[i] != 0 && buf[i] != '\n')
						{
							gSynchConsole->Write(buf + i, 1);
							i++;
						}
						buf[i] = '\n';
						gSynchConsole->Write(buf + i, 1);
						machine->WriteRegister(2, i - 1);
						delete buf;
						IncreasePC();
						return;
					}
				}
				case SC_Seek:{
					int pos = machine->ReadRegister(4);
					int id = machine->ReadRegister(5);
					if (id < 0 || id > 14){
						printf("\nFile nam ngoai bang mo ta.");
						IncreasePC();
						return;
					}
					// Kiem tra co goi Seek tren console khong
					if (id == 0 || id == 1){
						printf("\nKhong the seek tren file console.");
						IncreasePC();
						return;
					}
					// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
					if (pos == -1)
						pos = fileSystem->table[id]->Length();
					if (pos > fileSystem->table[id]->Length() || pos < 0){
						printf("\nVi tri khong hop le.");
						machine->WriteRegister(2, -1);
						IncreasePC();
						return;
					}
					else{
						fileSystem->table[id]->Seek(pos);
						machine->WriteRegister(2, pos);
					}
					IncreasePC();
					return;
				}
				case SC_Remove:{
					int virtAddr;
					char *filename;

					virtAddr = machine->ReadRegister(4);
					filename = User2System(virtAddr, MaxFileLength + 1);

					if (strlen(filename) == 0){
						printf("\nFile name is not valid");
						machine->WriteRegister(2, -1);
						delete filename;
						IncreasePC();
						return;
					}

					if (filename == NULL){
						printf("\nNot enough memory in system");
						machine->WriteRegister(2, -1);
						delete filename;
						IncreasePC();
						return;
					}

					if (!fileSystem->Remove(filename)){
						printf("\nError remove file '%s'", filename);
						machine->WriteRegister(2, -1);
						delete filename;
						IncreasePC();
						return;
					}

					machine->WriteRegister(2, 0);
					delete filename;
					printf("\nRemove file success!");
					IncreasePC();
					return;
				}
				default:
					break;
			}
			IncreasePC();
	}
}
