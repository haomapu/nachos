#include "syscall.h"

int main()
{
    int i;
    char c;
    char s[100];
    int mode;
    PrintString("\n===== Menu =====");
    PrintString("\n1. Integer");
    PrintString("\n2. Character");
    PrintString("\n3. Random number");
    PrintString("\n4. String");
    PrintString("\n5. Create file");
    PrintString("\n6. Open and Close file");
    PrintString("\nEnter mode: ");
    mode = ReadNum();
    if(mode == 1)
    {
        PrintString("Input a integer: ");
        i = ReadNum();
        PrintString("Your integer: ");
        PrintNum(i);

    }
    else if(mode == 2)
    {
        PrintString("Input a character: ");
        c = ReadChar();
        PrintString("Your character: ");
        PrintChar(c);

    }
    else if(mode == 3)
    {
        PrintString("The random number: ");
        PrintNum(RandomNum());

    }
    else if(mode == 4)
    {
        PrintString("Input a string: ");
        ReadString(s,100);
        PrintString("Your string: ");
        PrintString(s);

    }
    else if(mode == 5)
    {
        Create("hoang.txt");
        PrintString("Success!!!");
    }
    else if(mode == 6)
    {
        int openFileId;
        int fileSize;
        char c; //Ky tu de in ra
        char fileName[32];
        int i; //Index for loop
        PrintString(" - Nhap vao ten file can doc: ");

        //Goi ham ReadString de doc vao ten file
        //Co the su dung Open(stdin), nhung de tiet kiem thoi gian test ta dung ReadString
        ReadString(fileName, 32);

        openFileId = Open(fileName, 1); // Goi ham Open de mo file
        PrintNum(openFileId);
        if (openFileId != -1) //Kiem tra Open co loi khong
        {
            Close(openFileId); // Goi ham Close de dong file
        }
        else
        {
            PrintString(" -> Mo file khong thanh cong!!\n\n");
        }
    }
}