
//********Tarea3.C*********
//
//Fecha de creación : 13/01/2015
//Asunto:
//Rev #00

static char task3St;	//variable de estado

//DEfinición de los posibles estados de tarea1St
#define ESTADO_DUMMY 	0		//Estado de reposo
#define ESTADO_1			1
#define ESTADO_2			2

//Imprementación de la función Tarea1_Init
void Task3_Init(void){
	
}

//Implementación de la función Tarea1_Run
void Task3_Run(void){
	switch(task3St){
		case ESTADO_DUMMY:
			
		break;
		case ESTADO_1:
			
		break;
		case ESTADO_2:
			
		break;
	}
}

//Implementación de la función Tarea1_Flush
void Task3_Flush(void){
	task3St = ESTADO_DUMMY;
}
