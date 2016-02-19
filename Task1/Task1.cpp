
//********Tarea1.C*********
//
//Fecha de creación : 13/01/2015
//Asunto:
//Rev #00

static char task1St;	//variable de estado

//Definición de los posibles estados de tarea1St
#define ESTADO_DUMMY 	0		//Estado de reposo
#define ESTADO_1			1
#define ESTADO_2			2

//Imprementación de la función Tarea1_Init
void Task1_Init(void){
	return;
}

//Implementación de la función Tarea1_Run
void Task1_Run(void){
	switch(task1St){
		case ESTADO_DUMMY:
			
		break;
		case ESTADO_1:
			
		break;
		case ESTADO_2:
			
		break;
	}
}

//Implementación de la función Tarea1_Flush
void Task1_Flush(void){
	task1St = ESTADO_DUMMY;
}
