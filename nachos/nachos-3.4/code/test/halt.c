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
	i = ReadInt();
	PrintInt(i);
    Halt();
<<<<<<< HEAD:nachos/nachos-3.4/code/test/halt.c
=======
    // Hoang test
    // Hai test
>>>>>>> 68fa2b6a1b3a3f8171886ba59c156228f0ca0f0a:test/halt.c
    /* not reached */
}
