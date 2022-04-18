#include "syscall.h"
#include "copyright.h"
#define MAX_LENGTH 32

int main()
{
	char fileName[MAX_LENGTH];
	PrintString("Enter Filename: ");
	ReadString(fileName, MAX_LENGTH);
	if (!Remove(fileName))
	{
		PrintString("--- Delete successfully ---\n");
	}
	else
	{
		PrintString("--- Failed to delete!! ---\n");
	}

	return 0;
}