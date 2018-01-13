#include "syscall.h" 

int main() {
	char buff[255];
	unsigned int i ;
	OpenFileId inConsole = OpenFile("stdin", 1);
	OpenFileId outConsole = OpenFile("stdout", 0);
	if (inConsole == -1 || outConsole == -1)
	{
		PrintString("\nCan not open console\n");
		return 0;
	}
	ReadFile(buff, 255, inConsole);
	WriteFile(buff, 255, outConsole);
	PrintString("\n");
	CloseFile(inConsole);
	CloseFile(outConsole);
	return 0;
}