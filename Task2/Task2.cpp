

//********Tarea2.C*********
//
//Fecha de creación : 13/01/2015
//Asunto:
//Rev #00

static char task2St;	//variable de estado

//DEfinición de los posibles estados de tarea1St
#define ESTADO_DUMMY 	0		//Estado de reposo
#define ESTADO_1			1
#define ESTADO_2			2

//Imprementación de la función Tarea1_Init
void Task2_Init(void){
	
}

//Implementación de la función Tarea1_Run
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

//Implementación de la función Tarea1_Flush
void Task2_Flush(void){
	task2St = ESTADO_DUMMY;
}
