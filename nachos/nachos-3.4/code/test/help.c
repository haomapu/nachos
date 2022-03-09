#include "syscall.h"

int main()
{
    PrintString("\n\nWelcome to our project!\n");
    PrintString("Members: \n");
    PrintString("Mai Vu Huy Hoang\t20127504\n");
    PrintString("Ngo Gia Hai\t20127017\n");
    PrintString("Pham Anh Hao\t20127155\n\n");

    PrintString("Sorting by using:\n\" ./userprog/nachos -rs 1023 -x ./test/sort \"\n");
    PrintString("Ascii table by using:\n\" ./userprog/nachos -rs 1023 -x ./test/halt \"\n");
}