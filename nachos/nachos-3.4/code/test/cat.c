#include "syscall.h"
#include "copyright.h"
#define MAX_LENGTH 32

int main()
{
	int openFileId;
	int fileSize;
	char c;
	char fileName[MAX_LENGTH];
	int i;
	PrintString(" - Enter filename: ");

	ReadString(fileName, MAX_LENGTH);

	openFileId = Open(fileName, 1);

	if (openFileId != -1)
	{
		// get fileSize
		fileSize = Seek(-1, openFileId);
		i = 0;
		// start Read

		Seek(0, openFileId);

		PrintString(" --- File content ---\n");
		for (; i < fileSize; i++)
		{
			Read(&c, 1, openFileId);
			PrintChar(c);
		}
		Close(openFileId);
	}
	else
	{
		PrintString("Error Opening File!!\n");
	}
	return 0;
}