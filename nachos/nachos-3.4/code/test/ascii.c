#include "syscall.h"

int main()
{
    int i;
    PrintString("\n\t\t\t==== BANG MA ASCII ====\n");
    PrintString("\tDec\tChar\t|\tDec\tChar\t|\tDec\tChar\n");
    for(i = 0; i < 72; i++)
        PrintChar('-');
    PrintChar('\n');
    for (i = 32; i < 64; ++i) 
    {
        PrintChar('\t');

        // Cot 1
        PrintNum(i);
        PrintChar('\t');
        PrintChar(i);
        PrintString("\t|\t");

        // Cot 2
        PrintNum(i + 32);
        PrintChar('\t');
        PrintChar(i + 32);
        PrintString("\t|\t");

        if (i + 64 != 127)
        {
            // Cot 3
            PrintNum(i + 64);
            PrintChar('\t');
            PrintChar(i + 64);
        }
        PrintChar('\n');
    }
    return 0;
}