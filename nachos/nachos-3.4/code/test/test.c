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
}