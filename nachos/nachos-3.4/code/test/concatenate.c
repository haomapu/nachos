#include "syscall.h"

#define MAX_LENGTH 32

int main() {
    int srcFileId;
	int destFileId;

	int fileSize;
	int i;
	char c;
	char source[MAX_LENGTH];
	char dest[MAX_LENGTH];

	PrintString(" - Enter the source file: ");
	ReadString(source, MAX_LENGTH);
	PrintString(" - Enter the destination file: ");
	ReadString(dest, MAX_LENGTH);
	srcFileId = Open(source, 1);

	if (srcFileId != -1)
	{
		destFileId = Open(dest, 0);
		if (destFileId != -1)
		{
			fileSize = Seek(-1, srcFileId);
			Seek(0, srcFileId);
			Seek(-1, destFileId);
			i = 0;

			for (; i < fileSize; i++)
			{
				Read(&c, 1, srcFileId);
				Write(&c, 1, destFileId);
			}

			PrintString(" - Concatenate success\n");
			Close(destFileId);
		}
		else
		{
			PrintString(" - Cannot create file!!\n");
		}

		Close(srcFileId);
	}
	else
	{
		PrintString("Error Opening File!!!");
	}

}