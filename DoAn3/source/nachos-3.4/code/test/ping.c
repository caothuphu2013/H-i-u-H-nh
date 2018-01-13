#include "syscall.h"

void main()
{	
	int i;	
	for (i = 0; i < 1000; i++){	
		Down("start");
		PrintChar('A');
		Up("stop");
	}
	Exit(0);
}