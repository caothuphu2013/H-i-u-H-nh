#include "syscall.h"

int main() {
    char name[100];
    int arr[10];
    int i;
    CreateFile("input.txt");
    for(i =0; i < 10; i++)
    {
        arr[i] = 0;
    }
    for(i = 0; i < 10; i++)
    {
        PrintString("\nMo file thu ");
        PrintInt(i);
        arr[i] = OpenFile("input.txt", 1);
    }
    CloseFile(arr[3]);
    OpenFile("input.txt",1);
    OpenFile("input.txt",1);
    return 0;
}