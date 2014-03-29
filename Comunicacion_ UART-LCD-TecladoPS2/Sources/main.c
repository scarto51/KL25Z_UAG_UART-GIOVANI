/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "PS2.h"
#include "LCD.h"
#include "UART.h"

int main(void)
{
	vfnLCD_Init();
	vfnPS2_Config();
	vfnUART_Config();
	
	for(;;) 
	{	 
		vfnKeyboardLCD();
	}
	
	return 0;
}

void vfnLCD_Init(void)
{
	// turn on clocks
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;
	
	//LCD pins
	PORTB_PCR0= PORT_PCR_MUX(1);
	PORTB_PCR1= PORT_PCR_MUX(1);
	PORTB_PCR2= PORT_PCR_MUX(1);
	PORTC_PCR0= PORT_PCR_MUX(1);
	PORTC_PCR1= PORT_PCR_MUX(1);
	PORTC_PCR2= PORT_PCR_MUX(1);      //configure as GPIO
	PORTC_PCR3= PORT_PCR_MUX(1);
	PORTC_PCR4= PORT_PCR_MUX(1);
	PORTC_PCR5= PORT_PCR_MUX(1);
	PORTC_PCR6= PORT_PCR_MUX(1);
	PORTC_PCR7= PORT_PCR_MUX(1);
	
	GPIOB_PDDR=GPIOB_EN_MASK | GPIOB_RS_MASK | GPIOB_RW_MASK; //set LCD enable,RS and RW as outputs
	GPIOC_PDDR=GPIOD_DATA_MASK; //set LCD data pins as outputs
	
	vfnLCD_Write(INS,CLEAR); //clear screen
	vfnLCD_Write(INS,0x0E);//turn on screen DUDA VALOR?
	vfnLCD_Write(INS,SECOND_LINE_EN);//enable second line 
	
	vfnLCD_Write(INS,UP);//
}
/*****************************************************************/

void vfnSet_RS(void)
{
	GPIOB_PSOR=GPIOB_RS_MASK; //Set RS pin 
}

/*****************************************************************/

void vfnClear_RS(void)
{
	GPIOB_PCOR=GPIOB_RS_MASK; //Clear RS pin 
}

/*****************************************************************/

void vfnEnable(void)
{
	short hwDelay=WAIT_TIME;// 
	
	GPIOB_PSOR=GPIOB_EN_MASK;  //set enable
	while(hwDelay--);  //wait time
	GPIOB_PCOR=GPIOB_EN_MASK; //clear enable
}

/*****************************************************************/

void vfnLCD_Write(char bINS, char bData)
{
	if(bINS) //if INS
	{
		vfnClear_RS();  //turn off RS to write INS
		GPIOC_PDOR=bData;//load data in output
		vfnEnable();    
		vfnSet_RS(); //turn on RS to write char
	}
	else //if char
	{
		GPIOC_PDOR=bData; //load data in output
		vfnEnable();
	}
}

/****************************************************************/

void vfnLCD_Write_Array(char bcounter,char * pbArray)// 
{
	while(bcounter--)//set number of writes
	{
		vfnLCD_Write(CHAR,* pbArray);//write char 
		pbArray++; //increase pointer  
	}
}
void vfnUART_Config(void)
{
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
		
	UART0_BDL = UART0_BDL_SBR(137);
	UART0_C2 = UART0_C2_TE_MASK;
	
	PORTA_PCR1= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK; 
	PORTA_PCR2= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK;
}


void vfnUART_Send(char bData)
{
   	while(!(UART0_S1&UART_S1_TDRE_MASK));
   	UART0_D = bData;
}

void vfnUART_Send_String(char *pbArray)
{
	while(*pbArray != '\0')
	{
		vfnUART_Send(*pbArray);
		pbArray++;	
	}
}

int i;

int gbByte = 0;
char gbReceive = 1;
char gbRF = 0;
char gabData[4];
char gabBCD[3];
char gbSend;
char gbData_Cnt;

char gbWrite  = 0;
char gbRelease = 0;
char gbMayus  = 0;
char gbShift  = 0;
char gbComand = 0;
int gbCount = 0;
int gbOffset = 0;

char gabSave[33]={"                                "};

char gabID[49]= 
{
	0x0E,0x16,0x1E,0x26,0x25,0x2E,0x36,0x3D,0x3E,0x46,0x45,0x4E,0x55,
	0x15,0x1D,0x24,0x2D,0x2C,0x35,0x3C,0x43,0x44,0x4D,0x54,0x5B,
	0x1C,0x1B,0x23,0x2B,0x34,0x33,0x3B,0x42,0x4B,0x4C,0x52,0x5D,
	0x61,0x1A,0x22,0x21,0x2A,0x32,0x31,0x3A,0x41,0x49,0x4A,0x29
};

char gabMin[49]=
{
	"|1234567890'¿qwertyuiop´+asdfghjklñ{}<zxcvbnm,.- "
};

char gabMay[49]=
{
	"|1234567890'¿QWERTYUIOP´+ASDFGHJKLÑ{}<ZXCVBNM,.- "	
};

char gabShiftMin[49]=
{
	0xDF,'!','"','#','$','%','&','/','(',')','=','?','¡',
	'Q','W','E','R','T','Y','U','I','O','P','¨','*',
	'A','S','D','F','G','H','J','K','L','Ñ','[',']',
	'>','Z','X','C','V','B','N','M',';',':','_',' '
};

char gabShiftMay[49]=
{
	0xDF,'!','"','#','$','%','&','/','(',')','=','?','¡',
	'q','w','e','r','t','y','u','i','o','p','¨','*',
	'a','d','d','f','g','h','j','k','l','ñ','[',']',
	'>','z','x','c','v','b','n','m',';',':','_',' '
};



void vfnPS2_Config(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	PORTD_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10);//QUE ES ESTO IRQC
	PORTD_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK;
	
	NVIC_ISER = 1<<31;
}
/****************************************************************************************/
void vfnMouse_Init(void)
{
	vfnPS2_Send(RESET,3);
	vfnPS2_Send(SET_REMOTE_MODE,1);
}
/****************************************************************************************/
void vfnGetKey(void)
{
	if(gbRF)//que es RF?
	{
		if(gbRelease)
		{
			gbRelease = 0;
			if((gabData[0] == 0x12) || (gabData[0] == 0x59))
			{
				gbShift = 0;
			}
		}
		else
		{
			if(gabData[0] == 0xF0)
			{
				gbRelease = 1;
			}	
			else if((gabData[0] == 0x12) || (gabData[0] == 0x59))
			{
				gbShift = 1;
			}
			else if(gabData[0] == 0x58)//rshift
			{
				if(gbMayus)
				{
					gbMayus = 0;
				}
				
				else
				{
					gbMayus = 1;
				}
			}
			else if(gabData[0] == 0x66)//bcksp
			{
				if(gbCount)
				{
					gbCount--;
					if(gbCount==15)
					{
						vfnLCD_Write(INS,UP+15);
						vfnLCD_Write(CHAR,' ');
						vfnLCD_Write(INS,BACKSPACE);
					}
					else
					{
						vfnLCD_Write(INS,BACKSPACE);
						vfnLCD_Write(CHAR,' ');
						vfnLCD_Write(INS,BACKSPACE);
					}
					gabSave[gbCount] = ' ';	
				}
			}
			else if(gabData[0] == 0x5A)
			{
				vfnUART_Send_String(&gabSave[0]);//
			}
			else
			{
				gbWrite=1;
			}
		}
		gbRF = 0;
	}
}

/****************************************************************************************/
void vfnKeyboardLCD(void)
{
	vfnGetKey();
	if(gbWrite)
	{
		if(gbCount<32)
		{	
			while((gabData[0]  !=  gabID[gbOffset]) &&  (gbOffset<49))
			{
				gbOffset++;
			}
			
			if(gbMayus&&gbShift)
			{
				vfnLCD_Write(CHAR,gabShiftMay[gbOffset]);
				gabSave[gbCount] = gabShiftMay[gbOffset];
			}
			
			else if(gbMayus)
			{
				vfnLCD_Write(CHAR,gabMay[gbOffset]);
				gabSave[gbCount] = gabMay[gbOffset];
			}
			
			else if(gbShift)
			{
				vfnLCD_Write(CHAR,gabShiftMin[gbOffset]);
				gabSave[gbCount] = gabShiftMin[gbOffset];
			}
			
			else
			{
				vfnLCD_Write(CHAR,gabMin[gbOffset]);
				gabSave[gbCount] = gabMin[gbOffset];
			}
			gbCount++;
			if(gbCount==16)
			{
				vfnLCD_Write(INS,DOWN);
			}
		}

		gbOffset = 0;
		gbWrite  = 0;
	}
}
/****************************************************************************************/

void vfnLCD_Write_Mouse(void)
{
	for(i=0;i<250000;i++);
	vfnPS2_Send(READ_DATA,4);
	
	
	vfnLCD_Write(INS,UP+2);
	if(gabData[1] & 0x10)
	{
		vfnBCD_Covert(~gabData[2],&gabBCD[0]);
		vfnLCD_Write(CHAR,'-');
	}
	else
	{
		vfnBCD_Covert(gabData[2],&gabBCD[0]);
		vfnLCD_Write(CHAR,' ');
	}			
	vfnLCD_Write(CHAR,gabBCD[0]+0x30);
	vfnLCD_Write(CHAR,gabBCD[1]+0x30);
	vfnLCD_Write(CHAR,gabBCD[2]+0x30);
	
	
	
	vfnLCD_Write(INS,DOWN+2);
	if(gabData[1] & 0x20)
	{
		vfnBCD_Covert(~gabData[3],&gabBCD[0]);
		vfnLCD_Write(CHAR,'-');
	}
	else
	{
		vfnBCD_Covert(gabData[3],&gabBCD[0]);
		vfnLCD_Write(CHAR,' ');
	}			
	vfnLCD_Write(CHAR,gabBCD[0]+0x30);
	vfnLCD_Write(CHAR,gabBCD[1]+0x30);
	vfnLCD_Write(CHAR,gabBCD[2]+0x30);
}
/****************************************************************************************/

void vfnPS2_Send(char bData, char bReceive)
{
	gbReceive = 0;//
	gbSend =  bData;
	
	NVIC_ICER =  1<<31;
	GPIOD_PDDR = 1<<5;
	for(i=0;i<10000;i++);
	
	GPIOD_PDDR|=  1;
	GPIOD_PDDR &= ~(1<<5);
	
	PORTD_ISFR = 1<<5;
	NVIC_ICPR = 1<<31;
	NVIC_ISER = 1<<31;
	while(gbByte<bReceive);
	gbByte = 0;
}
/****************************************************************************************/

void vfnBCD_Covert(char bData, char *pbBCD)
{
	char c = 0;
	char d = 0;
	char u = 0;
	
	while(bData>=100)
	{
		bData = bData-100;
		c++;
	}
	while(bData>=10)
	{
		bData = bData-10;
		d++;
	}
	while(bData>=1)
	{
		bData--;
		u++;
	}
	pbBCD[0]=c;
	pbBCD[1]=d;
	pbBCD[2]=u;
}
/****************************************************************************************/

void PORTD_IRQHandler(void)
{
	if(gbReceive)
	{
		if(gbData_Cnt == 0)
		{
			gabData[gbByte] = 0;
		}
		else if (gbData_Cnt<=8)
		{
			gabData[gbByte] |= (GPIOD_PDIR&1) << (gbData_Cnt-1);
		}
		
		gbData_Cnt++;
		if(gbData_Cnt>10)
		{
			gbRF = 1;
			gbData_Cnt = 0;
		}
	}
	
	else 
	{
		if (gbData_Cnt<=7)
		{
			GPIOD_PDOR = (gbSend>>(gbData_Cnt))&1;
		}
		else if(gbData_Cnt == 8)
		{
			GPIOD_PDOR = ~((1&(gbSend))
		                  ^(1&(gbSend>>1))
		                  ^(1&(gbSend>>2))
		                  ^(1&(gbSend>>3))
		                  ^(1&(gbSend>>4))
		                  ^(1&(gbSend>>5))
		                  ^(1&(gbSend>>6))
		                  ^(1&(gbSend>>7)));
		}
		else if(gbData_Cnt == 9)
		{
			GPIOD_PDDR &= ~1;
		}

		gbData_Cnt++;
		if(gbData_Cnt>10)
		{
			gbData_Cnt = 0;
			gbReceive = 1;
			GPIOD_PDOR = 0;
		}
	}

	PORTD_ISFR = 1<<5;
}
