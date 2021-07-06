// SSD_with_external_interrupt.c



//Stable visitor counter (removing flickering & debounce problem)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SSDPORT PORTB
#define SLPORT PORTC
#define SL1 PC1
#define SL2 PC2
#define SL3 PC3
#define SL4 PC4

//Function Declaration (with 4 arguments but no return type)
void display(int d1,int d2,int d3,int d4);

void inc_data();	//Function declaration to increment data

//Array holding patterns 0	  1		2	3	4	  5		6	7	8	 9
unsigned int digit[10]={0XFC,0X60,0XDA,0XF2,0X66,0XB6,0XBE,0XE0,0XFE,0XF6};

int data1,data2,data3,data4;	//4 global variables to hold data

int main(void)
{
	DDRD=0XFF;			//Define PORTD as output (SSD data lines are connected)
	DDRC=0b0011110;		//Define PC1, PC2, PC3, PC4 as Output (Select lines)
			
	sei();							//Global interrupt enabled
	GICR=(1<<INT1)|(1<<INT0);		//External Interrupt 1 & 0 (local interrupt) Enabled

	MCUCR=(1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00);	//Interrupt On Rising Edge

	while (1)
	{			
		display(data1,data2,data3,data4); //Function to display data on SSD
	}
}

//display Function definition
void display(int d1,int d2,int d3,int d4)
{
	SLPORT=1<<SL2|1<<SL3|1<<SL4;	//SSD1 active
	SSDPORT=digit[d1];				//Data transmitted to SSD1
	_delay_ms(1);

	SLPORT=1<<SL1|1<<SL3|1<<SL4;	//SSD2 active
	SSDPORT=digit[d2];				//Data transmitted to SSD2
	_delay_ms(1);

	SLPORT=1<<SL1|1<<SL2|1<<SL4;	//SSD3 active
	SSDPORT=digit[d3];				//Data transmitted to SSD3
	_delay_ms(1);

	SLPORT=1<<SL1|1<<SL2|1<<SL3;	//SSD4 active
	SSDPORT=digit[d4];				//Data transmitted to SSD4
	_delay_ms(1);
}

//increment_data function definition
void inc_data()
{
	data4++;		//increment data4
	if (data4>9)
	{
		data4=0;	//if data4=10, reset data4 to 0
		data3++;	//increment data3
		
		if (data3>9)
		{
			data3=0;	//if data3=10, reset data3 to 0
			data2++;	//increment data2
			
			if (data2>9)
			{
				data2=0;	//if data2=10, reset data2 to 0
				data1++;	//increment data1
				
				if (data1>9)	//if data1=10
				{
					data1=data2=data3=data4=9;	//Keep all of them 9999.
				}
			}
		}
	}
}


ISR(INT1_vect)		//This ISR will be executed when button goes high from low
{
	data1=data2=data3=data4=0;	//Reset all data to 0	
}

ISR(INT0_vect)		//This ISR will be executed when button goes high from low
{	
	inc_data();			//increment your data by 1
}




























