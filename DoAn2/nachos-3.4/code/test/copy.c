#include "syscall.h"

int main() {
	char source[100],dest[100];
	OpenFileId sourceid,destid;
	char buff[255];

	PrintString("Nhap file nguon : ");
	ReadString(source,100);
	sourceid = OpenFile(source,1);
	if (sourceid == -1) {
		PrintString("\n");
		return 0;
	}

	PrintString("\nNhap file dich : ");
	ReadString(dest,100);
	CreateFile(dest);
	destid = OpenFile(dest,0);

	
	if (destid == -1) {
		PrintString("\n");
		return 0;
	}


	SeekFile(0,sourceid);
	SeekFile(0,destid);

	ReadFile(&buff,255,sourceid);
	WriteFile(buff,1000,destid);
	PrintString("\n");
	CloseFile(sourceid);
	CloseFile(destid);

	return 0;
}