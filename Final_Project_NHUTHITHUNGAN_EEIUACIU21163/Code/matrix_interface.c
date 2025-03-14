
#define F_CPU 16000000UL       	// XTAL = 16MHZ = 16000000Hz
#include <avr/io.h>    		// Standard AVR header
#include <util/delay.h> 	// Delay loop functions
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>



#define LCD_DATA PORTD // port connected to LCD  data pins
#define DATA_DDR DDRD  // direction register for data pins
#define LCD_CTRL PORTD // port connected to LCD control pins
#define CTRL_DDR DDRD // direction register for control pins

#define LCD_RS 0 // define MCU pin connected to LCD RS
#define LCD_RW 1 // define MCU pin connected to LCD R/W
#define LCD_E  2 // define MCU pin connected to LCD E

#define LCD_D0 0 // define MCU pin connected to LCD D0
#define LCD_D1 1 // define MCU pin connected to LCD D1
#define LCD_D2 2 // define MCU pin connected to LCD D1
#define LCD_D3 3 // define MCU pin connected to LCD D2
#define LCD_D4 4 // define MCU pin connected to LCD D3
#define LCD_D5 5 // define MCU pin connected to LCD D4
#define LCD_D6 6 // define MCU pin connected to LCD D5
#define LCD_D7 7 // define MCU pin connected to LCD D6




void LCD_Command(unsigned char cmd){ //Sends Command to LCD
	//4 bit part
	LCD_DATA=(cmd&0b11110000); // send upper 4-bits
	LCD_CTRL|=1<<LCD_E;		// E=1 ,RS=0, RW=0
	_delay_ms(1);	// keep E=1 for some time
	LCD_CTRL&=~(1<<LCD_E);  // E=0 ,RS=0, RW=0
	_delay_ms(1); // keep E=0 for some time

	LCD_DATA=((cmd&0b00001111)<<4);	// send lover 4-bits
	LCD_CTRL|=1<<LCD_E;	 // E=1,RS=0,RW=0
	_delay_ms(1); // keep E=1 for some time
	LCD_CTRL&=~(1<<LCD_E); // E=0,RS=0,RW=0
	_delay_ms(1); // keep E=0 for some time
}

void delay1s(void){ //delay 1s
	unsigned char i;
	for(i=0;i<100;i++){
		_delay_ms(10);
	}
}

void LCD_Show(uint8_t ch){		//Sends Char to LCD
	LCD_DATA=(ch&0b11110000); // send upper 4-bits
	LCD_CTRL|=(1<<LCD_E)|(1<<LCD_RS); // E=1, RS=1
	_delay_ms(1);  // keep E=1 for some time
	LCD_CTRL&=~((1<<LCD_E)); // E=0
	_delay_ms(1);  // keep E=0 for some time

	LCD_DATA=((ch&0b00001111)<<4); // send lower 4-bits
	LCD_CTRL|= (1<<LCD_E)|(1<<LCD_RS); // E=1, RS=1
	_delay_ms(1); // keep E=1 for some time	
	LCD_CTRL&=~(1<<LCD_E);  // E=0
	_delay_ms(1); // keep E=0 for some time
}

void LCD_init(void)
{ //Initializes LCD
	_delay_ms(15);
	LCD_DATA=0x00; // data = 0
	LCD_CTRL=0x00; // RS = RW = E = 0
	DATA_DDR|=1<<LCD_D7|1<<LCD_D6|1<<LCD_D5|1<<LCD_D4;
	// Set bits 4 to 7 as output pins for data out 
	CTRL_DDR|=1<<LCD_E|1<<LCD_RW|1<<LCD_RS;
	// Set bit 0 to 2 as output pins
	//---------one------ // DATA = 0x30 ; 
	LCD_DATA = 0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4; //4 bit mode
	// E=1, RW=0, RS=0 for command mode
	LCD_CTRL|= 1<<LCD_E|0<<LCD_RW|0<<LCD_RS;		
	_delay_ms(1); // keep E=1 for some time
	// E=0;
	LCD_CTRL&=~(1<<LCD_E);
	_delay_ms(1); // keep E=0 for some time
	//-----------two-----------  // DATA = 0x30 ; 
	LCD_DATA=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4; 
	//4 bit mode
	// E=1, RW=0, RS=0 for command mode
	LCD_CTRL|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;		
	_delay_ms(1); // keep E=1 for some time
	LCD_CTRL&=~(1<<LCD_E); // E=0
	_delay_ms(1); // keep E=0 for some time
	//-------three-------------
	// DATA = 0x20 ; 
	LCD_DATA=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|0<<LCD_D4; //4 bit mode
	// E=1, RW=0, RS=0 for command mode
	LCD_CTRL|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;		
	_delay_ms(1); // keep E=1 for some time
	LCD_CTRL&=~(1<<LCD_E); // E=0
	_delay_ms(1); // keep E=0 for some time
	//--------4 bit--dual line---------------
	LCD_Command(0b00101000); // 0x28
	//-----increment address, invisible cursor shift------
	LCD_Command(0b00001100); // 0x0C 
	LCD_Command(0b10000000); // 0x80
}

void LCD_CustomChar() {
	LCD_Command(0xC9); 	// Cursor at line 2 col 9
	LCD_Show(0x00);	/* Show Character at CGRAM address 0 */
	LCD_Show(0x01);	/* Show Character at CGRAM address 1 */
	LCD_Show(0x02);	/* Show Character at CGRAM address 2 */
	LCD_Show(0x03);	/* Show Character at CGRAM address 3 */
	LCD_Command(0x80); /* select DDRAM location 4*/
}

//Clear Display
void lcd_clear(void)
{
	LCD_Command(0x01);//Clear
}

void lcd_cursor (char row, char column)
{

  switch (row)

  {

    case 1: LCD_Command (0x80 + column - 1); break;

    case 2: LCD_Command (0xc0 + column - 1); break;

    default: break;

  }

}

void lcd_display_string (char row, char column , char string[])
{

  lcd_cursor (row, column);

  while (*string)

    LCD_Show(*string++);

}

void My_LCD_Display_String(char row, char column,char *str)		
{
	int i;
	lcd_cursor (row, column);
	for(i=0;str[i]!=0;i++)  /* send each char of string till the NULL */
	{
		LCD_Show (str[i]);  /* call LCD data write */
	}
}


static unsigned int key;
static unsigned int new_key;
static unsigned int op;
static unsigned int operand;
static char char_op,temp;
char buf_lcd1[]={};

#define KEYPAD_PORT   PORTC
#define KEYPAD_DDR    DDRC
#define KEYPAD_PIN    PINC

void Keypad_process()  ;

// Keypad layout
unsigned char keypad[4][4] = {
    {1, 5, 9, 13},	 // 1 2 3 A
    {2, 6, 10,14},   // 4 5 6 B
    {3, 7, 11,15},	// 7 8 9 C
    {4, 8, 12,16} 	// * 0 # D
};
//-------------------------------------------------------------//
// External Interrupt 0 service routine
// key stores the pressed ky by user.
void readKeypad()
{   
    unsigned char r, c;
    // Set all keypad column pins as input with pull-up resistors
    KEYPAD_DDR = 0xF0;
    KEYPAD_PORT = 0x0F;
    // Loop through each row and check for key press
    for (r = 0; r < 4; r++) {
        // Set current row as output low
        KEYPAD_DDR = (0x80 >> r);
        KEYPAD_PORT = ~(0x80 >> r);
        _delay_ms(2);  // Delay for stable readings

        // Check each column for key press
        for (c = 0; c < 4; c++) {
            if (!(KEYPAD_PIN & (0x08 >> c))) {
                // Key pressed, return corresponding character
				//A new key has been pressed
				new_key=1;
                temp=keypad[r][c];
				Keypad_process();			
            }
        }
    }
}
 

void Keypad_process()  
{ 
	//Determine what key
	if(temp==1) //temp = keypad[r][c]
	 {
		key=1;
     }	
	else if(temp==2)
	 {
		key=4;
     }
		
	else if(temp==3)
	 {
		key=7;
     }
				
	else if(temp==5)
	 {
		key=2;

     }
		
	else if(temp==6)
	 {
		key=5;

     }
		
	else if(temp==7)
	 {
		key=8;

     }		
	else if(temp==8)
	 {
		key=0;
     }
		
	else if(temp==9)
	 {
		key=3;

     }
		
	else if(temp==10)
	 {

		key=6;

     }

	 else if(temp==11)
	 {

		key=9;

     }
		
	else if(temp==13)
	{
		key=0;//add
		op=1;
		char_op='+';
		operand++;
	}
		
	else if(temp==14)
	{	
		key=0;//subtraction
		op=2;
		char_op='-';
		operand++;
	}		
		
	else if(temp==15)
	{
		key=0;//mult
		op=3;
		char_op='x';
		operand++;
	}		

	else if(temp==16)
	{		
		key=0;//div
		op=4;
		char_op='/';
		operand++;
	}		
		
	else if(temp==12)
	{	
		key=0;//execute
		op=5;
	}		
	
	else if(temp==4)
	{
		key=0;//on/c
		op=6;
	}		
		
}
//-------------------------------------------------------------//


//-------------------------------------------------------------//
//Configure ports as inputs and outputs.
void ports_init(void)
{
    // set port A for output
    DDRA = 0xFF;
    // set port B for output
    DDRB = 0xFF; 
    // set port C for input 
    DDRC = 0x00; 

}

//-------------------------------------------------------------//

//-------------------------------------------------------------//
int main(void) 
{

	int i;
	long a, b;
	double result;
	
	operand=0;
	new_key=0;
	key=0;
	a=0;
	b=0;
	result=0;
	op=0;
	char_op='+';
	
	//Init port pins
	ports_init();
    
	//Initialize lcd
	LCD_init();
	LCD_Command(0x0C);

	//--------------------------------------//
	//Send wellcome message
	sprintf(buf_lcd1, "My calculator");
	lcd_display_string(1,2,buf_lcd1);
	_delay_ms(5);
	
	sprintf(buf_lcd1, "Thu Ngan");
	lcd_display_string(2,2,buf_lcd1);
	_delay_ms(5);
	//--------------------------------------//
	
	
	//--------------------------------------//
	//Shifts Message
	for(i=0; i<16; i++)
	{
		//lcd_send_command (0x18);//left
		LCD_Command (0x1C);//right
		_delay_ms(5);		

	}
	//--------------------------------------//
	
	//Clear lcd
	lcd_clear();
	sprintf(buf_lcd1, "0");
	lcd_display_string(1,1,buf_lcd1);
		
	while(1)
	{
		readKeypad();
		if(new_key==1)
		{
			//first operand
			if(op==0)
			{
				a=a*10+key;
				sprintf(buf_lcd1, "%ld",a);
				lcd_display_string(1,1,buf_lcd1);
			}
			
			//second operand
			else if(op==1 || op==2 || op==3 || op==4)
			{
				b=b*10+key;
				sprintf(buf_lcd1, "%ld%c%ld",a,char_op,b);
				lcd_display_string(1,1,buf_lcd1);
			}
			
			//execute operation			
			else if(op==5)
			{
				if(char_op=='+')
					result=(double)a+(double)b;
				//	result = a + b;
					
				else if(char_op=='-')
					result=(double)a-(double)b;	
					//result = a - b;				
					
				else if(char_op=='x')
					result=(double)a*(double)b;		
				//result = a * b;					
					
				else if(char_op=='/')
					result=(double)a/(double)b;	
				//result = a / b;			
	
				//Convert double,float value to string to display on LCD sonce LCD cannot display double , float datatype
				//dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
				dtostrf(result,1,2,buf_lcd1);
				//lcd_display_string(2,1,buf_lcd1);
				My_LCD_Display_String(2,1,buf_lcd1);
			}
			
			//reset values
			else if(op==6)
			{
				a=0;
				b=0;
				result=0;
				operand=0;
				lcd_clear();
				sprintf(buf_lcd1,"0");
				lcd_display_string(1,1,buf_lcd1);
				op=0;
			}

			new_key=0;
		}
			
		asm("nop");
		
	} 

}
//-------------------------------------------------------------//


