#include "syscall.h"

void bubbleSortAsc(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n-1; i++){
        for (j = 0; j < n - i - 1; j++){
            if (arr[j] > arr[j + 1]){
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bubbleSortDes(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n-1; i++){
        for (j = 0; j < n - i - 1; j++){
            if (arr[j] < arr[j + 1]){
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void inputArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        PrintString("a[");
        PrintNum(i);
        PrintString("] ");
        PrintString(": ");
        arr[i] = ReadNum();
    }
}
 
void printArray(int arr[], int n)
{
    int i;
    for (i=0; i < n; i++)
    {
        PrintNum(arr[i]);
        PrintChar(' ');
    }
    PrintChar('\n');
}
 
int main()
{
    int arr[10];
    int n, mode;
    PrintString("Enter length of array: ");
    n = ReadNum();
    inputArray(arr, n);
    PrintString("Array: \n");
    printArray(arr, n);
    PrintString("1. Ascending\n");
    PrintString("2. Descending\n");
    PrintString("Enter mode: ");
    mode = ReadNum();
    if(mode == 1){
        bubbleSortAsc(arr, n);
    }
    else if(mode == 2){
        bubbleSortDes(arr, n);
    }
    else{
        PrintString("Error!\n");
    }
    PrintString("\nArray is sorted: \n");
    printArray(arr, n);
}