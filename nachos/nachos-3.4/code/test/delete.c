#include "syscall.h"
#include "copyright.h"
#define MAX_LENGTH 32

int main()
{
	char fileName[MAX_LENGTH];
	PrintString("Nhap ten file can xoa: ");
	ReadString(fileName, MAX_LENGTH);
	if (!Remove(fileName))
	{
		PrintString("--- Xoa file thanh cong ---\n");
	}
	else
	{
		PrintString("--- Xoa file that bai ---\n");
	}

	return 0;
}