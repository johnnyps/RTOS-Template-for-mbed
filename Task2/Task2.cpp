

//********Tarea2.C*********
//
//Fecha de creaci�n : 13/01/2015
//Asunto:
//Rev #00

static char task2St;	//variable de estado

//DEfinici�n de los posibles estados de tarea1St
#define ESTADO_DUMMY 	0		//Estado de reposo
#define ESTADO_1			1
#define ESTADO_2			2

//Imprementaci�n de la funci�n Tarea1_Init
void Task2_Init(void){
	
}

//Implementaci�n de la funci�n Tarea1_Run
void Task2_Run(void){
	switch(task2St){
		case ESTADO_DUMMY:
			
		break;
		case ESTADO_1:
			
		break;
		case ESTADO_2:
			
		break;
	}
}

//Implementaci�n de la funci�n Tarea1_Flush
void Task2_Flush(void){
	task2St = ESTADO_DUMMY;
}
