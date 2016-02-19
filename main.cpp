#pragma diag_suppress 550
#include "mbed.h"
#include "Tasks.h"
#include "Gps.h"

void Mcu_Init(void);
void EnInterrupts(void);
int main()
{
	Mcu_Init();
	Task1_Init();
	Task2_Init();
	Task3_Init();
	Task4_Init();
	
	EnInterrupts();
	while(1)
	{	
		Task1_Run();
		Task2_Run();
		Task3_Run();
		Task4_Run();
		wait_ms(500);
	}
 
}

void Mcu_Init(void){
	
	return;
}

void EnInterrupts(void){
	return;
}
