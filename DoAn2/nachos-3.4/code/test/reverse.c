#include "syscall.h"

int main() {
	char source[100],dest[100];
	OpenFileId sourceid,destid;
	char buff[255], buffw[255];
	int sourcesz,i,j;

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

	sourcesz = ReadFile(&buff,255,sourceid);
	j = 0;
	for (i = sourcesz - 1;i >= 0;i--) {
		buffw[j] = buff[i];
		j++;
	}

	WriteFile(buffw,255,destid);
	PrintString("\n");
	CloseFile(sourceid);
	CloseFile(destid);

	return 0;
}