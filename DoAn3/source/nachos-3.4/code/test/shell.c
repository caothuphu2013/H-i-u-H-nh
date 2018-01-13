
#include "syscall.h"

int main()
{
	int pingPID, pongPID, i;
	
	CreateSemaphore("start", 1);
	CreateSemaphore("stop", 0);
	
	PrintString("\nPing-Pong test starting ...\n");
	
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");

	Join(pingPID);
	Join(pongPID);
	

	return 0;
}


/*
#include "syscall.h"

int
main()
{
    SpaceId newProc;
  
    char prompt[2], ch, buffer[60];
    int i;

    prompt[0] = '-';
    prompt[1] = '-';

    while( 1 )
    {
	PrintString(prompt)

	i = 0;
	
	do {
	
	    ReadString(&buffer[i], 1, input); 


	if( i > 0 ) {
		newProc = Exec(buffer);
		Join(newProc);
	}
    }
}

*/