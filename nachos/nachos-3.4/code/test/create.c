#include "syscall.h"

int main()
{
    int mode;
    PrintString("\n1. Default\n");
    PrintString("2. Your Input\n");
    PrintString("Enter mode: ");
    mode = ReadNum();
    if(mode == 1)
    {
        Create("mydata.txt");
        PrintString("Create file mydata.txt success!");
    }
    else
    {
        char s[100];
        PrintString("Input your name's file: ");
        ReadString(s,100);
        Create(s);
        PrintString("Create file ");
        PrintString(s);
        PrintString(" success!");
    }

}