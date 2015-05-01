#include <ModbusSlave232.h>
/*

 # Editor : Phoebe
 # Date   : 2012.11.6
 # Ver    : 0.1
 # Product: Cherokey 4WD Mobile Platform
 # SKU    : RBO0102
   
 # Description:     
 # Drive 2 motors with this Cherokey 4WD Mobile Platform  
 # Connect the D4,D5,D6,D7,GND to UNO digital 4,5,6,7,GND
 
*/
 //Variables  
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M1 Direction Control
int PWM_X1 = 0;
int PWM_X2 = 0;

//---------------------------------------------ENCABEZADO MODBUS----------------------------------------------------------------------
// Create new mbs instance
ModbusSlave232 mbs;
enum {   
  MB_EJE_X1,  //DIRECCION 1
  MB_EJE_X2,  //DIRECCION 2
  MB_LIFE_SIGNAL,//DIRECCION 3
  MB_REGS // Dummy register. using 0 offset to keep size of array
};
int regs[MB_REGS];
//------------------------------------------------------------------------------------------------------------------------------------- 

 
void setup(void) 
{ 
  int i;
  for(i=4;i<=7;i++)			//define los pines 4 a 7 como salidas
    pinMode(i, OUTPUT);  
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW); 

  // Modbus slave configuration parameters
  // SlaveId 
  const unsigned char SLAVE = 1;
  // Baud rate   
  const long BAUD = 9600;            
  const unsigned PARITY = 'n';

  // Configure msb with config settings
  mbs.configure(SLAVE, BAUD, PARITY ); 
  
  regs[MB_EJE_X1] = 120; //fix arranque
  regs[MB_EJE_X2] = 120;
} 
  
void loop(void) 
{
    // Pass current register values to mbs
  mbs.update(regs, MB_REGS);
//-------------------------------------eje x1-------------------------------------------------------------------------------------------------
	if (regs[MB_EJE_X1] > 150)				//Adelante
	{  
		PWM_X1 = map(regs[MB_EJE_X1], 150, 255, 60, 255); //escalado desde 60 a 255 por velocidad minima
		analogWrite (E1,PWM_X1); //PWM Speed Control
		digitalWrite(M1,HIGH); 
	}
	else if (regs[MB_EJE_X1] < 100)			//Reversa
	{
		PWM_X1 = map(regs[MB_EJE_X1], 100, 0, 60, 255);
		analogWrite (E1,PWM_X1); //PWM Speed Control
		digitalWrite(M1,LOW);   
	}
	else									//El eje se encuentra en la "zona muerta"
	{
		digitalWrite(E1,0);				 	//Parar
		digitalWrite(M1,LOW);
	}
//-------------------------------------eje x2-------------------------------------------------------------------------------------------------
	if (regs[MB_EJE_X2] > 150)				//Adelante
	{  
		PWM_X2 = map(regs[MB_EJE_X2], 150, 255, 60, 255);
		analogWrite (E2,PWM_X2); //PWM Speed Control
		digitalWrite(M2,HIGH); 
	}
	else if (regs[MB_EJE_X2] < 100)			//Reversa
	{
		PWM_X2 = map(regs[MB_EJE_X2], 100, 0, 60, 255);	
		analogWrite (E2,PWM_X2); //PWM Speed Control
		digitalWrite(M2,LOW);   
	}
	else									//El eje se encuentra en la "zona muerta"
	{
		digitalWrite(E2,0); 				//Parar
		digitalWrite(M2,LOW);
	}	
	
  delay(10);	
}