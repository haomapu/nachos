#include "syscall.h"

void bubbleSort(int arr[], int n)
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
void inputArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        PrintString("So thu ");
        PrintNum(i);
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
    int n;
    PrintString("Do dai cua mang: ");
    n = ReadNum();
    inputArray(arr, n);
    PrintString("Mang sau khi nhap: \n");
    printArray(arr, n);
    bubbleSort(arr, n);
    PrintString("Mang sau khi sort: \n");
    printArray(arr, n);
}