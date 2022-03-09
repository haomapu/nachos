/* halt.c
 *	Simple program to test whether running a user program works.
 *
 *	Just do a "syscall" that shuts down the OS.
 *
 * 	NOTE: for some reason, user programs with global data structures
 *	sometimes haven't worked in the Nachos environment.  So be careful
 *	out there!  One option is to allocate data structures as
 * 	automatics within a procedure, but if you do this, you have to
 *	be careful to allocate a big enough stack to hold the automatics!
 */

#include "syscall.h"

int main()
{

    //them test
    int i;
    char c;
	i = ReadNum();
	PrintNum(i);
    PrintNum(RandomNum());

    c = ReadChar();

    i = 5;

    //help 
    PrintString("Welcome to our project!\n");
    PrintString("Members: \n");
    PrintString("Mai Vu Huy Hoang\t20127504\n");
    PrintString("Ngo Gia Hai\t20127017\n");
    PrintString("Pham Anh Hao\t20127155\n\n");

    PrintString("Sorting by using:\n \" ./userprog/nachos -rs 1023 -x ./test/sort \"");
    PrintString("Ascii table by using:\n \" ./userprog/nachos -rs 1023 -x ./test/halt \"");

    // Ten bang ma ASCII
    PrintChar('\n');

    // In cac ten cot trong bang
    PrintChar('\t');
    PrintString("Dec");
    PrintChar('\t');
    PrintChar('C');
    PrintChar('h');
    PrintChar('a');
    PrintChar('r');

    PrintChar('\t');
    PrintChar('|');
    PrintChar('\t');
    PrintChar('D');
    PrintChar('e');
    PrintChar('c');
    PrintChar('\t');
    PrintChar('C');
    PrintChar('h');
    PrintChar('a');
    PrintChar('r');

    PrintChar('\t');
    PrintChar('|');
    PrintChar('\t');
    PrintChar('D');
    PrintChar('e');
    PrintChar('c');
    PrintChar('\t');
    PrintChar('C');
    PrintChar('h');
    PrintChar('a');
    PrintChar('r');
    PrintChar('\n');

    // In duong vien cua bang
    for(i = 0; i < 72; i++)
        PrintChar('-');
    PrintChar('\n');

    // In bang ASCII thanh 3 cot
    
    for (i = 32; i < 64; ++i)     // Cac ki tu doc duoc tu 32 toi 126
    {
        PrintChar('\t');

        // Cot 1
        PrintNum(i);
        PrintChar('\t');
        PrintChar(i);

        PrintChar('\t');
        PrintChar('|');
        PrintChar('\t');

        // Cot 2
        PrintNum(i + 32);
        PrintChar('\t');
        PrintChar(i + 32);

        PrintChar('\t');
        PrintChar('|');
        PrintChar('\t');

        if (i + 64 != 127)    // Neu ki tu gia tri 127 thi khong in
        {
            // Cot 3
            PrintNum(i + 64);
            PrintChar('\t');
            PrintChar(i + 64);
        }
        PrintChar('\n');
    }
    Halt();
  /* not reached */
}
