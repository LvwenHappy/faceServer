#include "CEpoll.h"
#include <stdio.h>

int main()
{
	printf("beginning...\n");
	CEpoll* pEpoll = new CEpoll();
	printf("CEpoll create success and into mainLoop!\n");
	pEpoll->mainLoop();

	return 0;
}