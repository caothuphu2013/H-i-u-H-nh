#include "syscall.h"

int main(){
    char s[100];
    PrintString("\nNhap ten file: ");
    ReadString(s,100);
    CreateFile(s);
    return 0;
}