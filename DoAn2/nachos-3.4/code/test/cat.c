#include "syscall.h"

int main() {
	char namefile[100];
	char buff[255];
	int id;
	PrintString("\nNhap vao ten file : ");
	ReadString(namefile,100);

	id = OpenFile(namefile,1);
	if (id < 0) {
		PrintString("\n");
		return 0;
	}
	
	SeekFile(0,id);
	ReadFile(&buff,255,id);
	PrintString(buff);

	PrintString("\n");
	CloseFile(id);
	return 0;
}