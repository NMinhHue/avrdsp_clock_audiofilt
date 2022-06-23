#define F_CPU7372800_UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "myDS1307RTC.h"
#define PIN_SCK                   1 // CKL
#define PIN_MOSI                  2 // DIN
#define PIN_SS                    0 // LOAD

#define PIN_SCL						PD0
#define PIN_SDA						PD1
#define DS1307_BASE_ADDRESS 		0xD0

// External crystal frequency
#define F_RTC           32768

// Define delay
#define PERIOD_MS       1000

#define MAX7219_LOAD1             PORTB &= ~(1<<PIN_SS) // choose mode
#define MAX7219_LOAD0             PORTB |= (1<<PIN_SS)

#define MAX7219_MODE_DECODE       0x09 // decode BCD
#define MAX7219_MODE_INTENSITY    0x0A // light
#define MAX7219_MODE_SCAN_LIMIT   0x0B //
#define MAX7219_MODE_POWER    	  0x0C // have two mode 0 or 1

#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

// address of register
#define MAX7219_DIGIT0            0x01
#define MAX7219_DIGIT1            0x02
#define MAX7219_DIGIT2            0x03
#define MAX7219_DIGIT3            0x04
#define MAX7219_DIGIT4            0x05
#define MAX7219_DIGIT5            0x06
#define MAX7219_DIGIT6            0x07
#define MAX7219_DIGIT7            0x08

#define MAX7219_CHAR_BLANK        0x0F
#define MAX7219_CHAR_NEGATIVE     0x0A

volatile int16_t Second;
volatile int16_t Minute;
volatile int16_t Hour;
volatile int16_t Date;
volatile int16_t Month;
volatile int16_t Year;

void spiSend_Byte (char databyte)
{
	// Copy data into the SPI data register
	SPDR = databyte;
	// Wait until transfer is complete
	while (!(SPSR & (1 << SPIF)));
}

void MAX7219_Write_Data(uint8_t data_register, uint8_t data)
{
	MAX7219_LOAD0;
	// Send the register where the data will be stored
	spiSend_Byte(data_register);
	// Send the data to be stored
	spiSend_Byte(data);
	MAX7219_LOAD1;
}

//dinh nghia bien chung
//dinh nghi cac bien thoi gian
//volatile uint8_t	Second=59, Minute=59, Hour=11,
//		Day=1, Date=31, Month=12, Year=9, Mode=1, AP=1;
//Mode: chon che do 12h hoac 24h, Mode nam o bit 6 cua thanh ghi HOURS,
//Mode=1: 12H, Mode=0: 24H
//AP: bien chi AM hoac PM trong Mode 12h, AP nam o bit 5 cua thang ghi HOURS,
//AP=1:PM, AP=0: AM

volatile  uint8_t tData[7], Time_count=0; //dinh nghia cac bien tam
char dis[5];		//bien dis dung de luu tru string hien thi len LCD

// doi BCD sang thap phan va nguoc lai------------
uint8_t BCD2Dec(uint8_t BCD)
{
	uint8_t L, H;
	L=BCD & 0x0F;
	H=(BCD>>4)*10;
	return (H+L);
}
uint8_t Dec2BCD(uint8_t Dec){
	uint8_t L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}
//------------------------------------------------
void Display_7seg (void)
{
	Second 	= BCD2Dec(tData[0] & 0x7F);
	Minute 	= BCD2Dec(tData[1]);

	Hour = BCD2Dec(tData[2] & 0x1F); //mode 12h


	Date   	= BCD2Dec(tData[4]);
	Month	= BCD2Dec(tData[5]);
	Year	= BCD2Dec(tData[6]);

// 	MAX7219_Write_Data(MAX7219_DIGIT7,(tData[2]/10));
// 	MAX7219_Write_Data(MAX7219_DIGIT6,(tData[2]%10));
// 
// //	MAX7219_writeData(MAX7219_DIGIT2,(??)); dau -
// 
// 	MAX7219_Write_Data(MAX7219_DIGIT4,(tData[1]/10));
// 	MAX7219_Write_Data(MAX7219_DIGIT3,(tData[1]%10));
// 
// //	MAX7219_writeData(MAX7219_DIGIT5,(??)); // dau -
// 
// 	MAX7219_Write_Data(MAX7219_DIGIT1,(tData[0]/10));
// 	MAX7219_Write_Data(MAX7219_DIGIT0,(tData[0]%10));
	MAX7219_Write_Data(MAX7219_DIGIT7,(Hour/10));
	MAX7219_Write_Data(MAX7219_DIGIT6,(Hour%10));

	//	MAX7219_writeData(MAX7219_DIGIT2,(??)); dau -

	MAX7219_Write_Data(MAX7219_DIGIT4,(Minute/10));
	MAX7219_Write_Data(MAX7219_DIGIT3,(Minute%10));

	//	MAX7219_writeData(MAX7219_DIGIT5,(??)); // dau -

	MAX7219_Write_Data(MAX7219_DIGIT1,(Second/10));
	MAX7219_Write_Data(MAX7219_DIGIT0,(Second%10));
}

void Display(void)
{ //chuong trinh con  hien thi thoi gian doc tu DS1307
//	Second 	= BCD2Dec(tData[0] & 0x7F);
//	Minute 	= BCD2Dec(tData[1]);
//
//	if (Mode !=0) 	Hour = BCD2Dec(tData[2] & 0x1F); //mode 12h
//	else 		  	Hour = BCD2Dec(tData[2] & 0x3F); //mode 24h
//
//	Date   	= BCD2Dec(tData[4]);
//	Month	= BCD2Dec(tData[5]);
//	Year	= BCD2Dec(tData[6]);
//
////	clr_LCD();		//xoa LCD
////	//in gio:phut:giay
////	print_LCD("Time: ");
//	sprintf(dis, "%i",Hour);
//	move_LCD(1,7);  print_LCD(dis); move_LCD(1,9); putChar_LCD('-');
//	sprintf(dis, "%i",Minute);
//	move_LCD(1,10); print_LCD(dis); move_LCD(1,12);putChar_LCD('-');
//	sprintf(dis, "%i",Second);
//	move_LCD(1,13); print_LCD(dis);
//	if (Mode !=0){ //mode 12h
//		move_LCD(1,16);
//		if (bit_is_set(tData[2],5))  putChar_LCD('P'); //kiem tra bit AP, if AP=1
//		else putChar_LCD('A');
//	}
//	//in nam-thang-ngay
//	move_LCD(2,1);
//	print_LCD("Date: ");
//	sprintf(dis, "%i",Year);
//	move_LCD(2,7);
//	if (Year<10) putChar_LCD('0'); // neu nam <10, in them so 0 ben trai, vi du 09
//	print_LCD(dis); move_LCD(2,9); putChar_LCD('-'); //in Nam
//	sprintf(dis, "%i",Month);
//	move_LCD(2,10); print_LCD(dis); move_LCD(2,12);putChar_LCD('-'); //in thang
//	sprintf(dis, "%i",Date);
//	move_LCD(2,13); print_LCD(dis);	//in Ngay
}

//chuong trinh chinh
int main(void)
{

	DDRB |= (1 << PIN_SCK)| (1 << PIN_SS)| (1 << PIN_MOSI);
	SPCR |=(1<<SPE) |(1<<MSTR)| (1<<SPR1);
	// DS1307 DATA DIRECTION
	DDRD |= (1 << PIN_SCL)| (1 << PIN_SDA);
	PORTD &= ~(1 << PIN_SCL)| (1 << PIN_SDA);

	MAX7219_Write_Data(MAX7219_MODE_DECODE, 0xFF);
	MAX7219_Write_Data(MAX7219_MODE_INTENSITY, 0x0A);
	MAX7219_Write_Data(MAX7219_MODE_SCAN_LIMIT, 0x07);
	MAX7219_Write_Data(MAX7219_MODE_POWER, 0x01);
	
	//khoi dong Timer0 lam bo dinh thi 1s------------------------------------------------
	TCCR0=(1<<CS02)|(0<<CS01)|(1<<CS00);	//CS02=1, CS01=0, CS00=1: chon Prescaler=1024
    TIMSK=(1<<TOIE0);						//cho phep ngat khi co tran o T/C0
    sei();                      			//set bit I cho phep ngat toan cuc
	//----------------------------------------------------------------

	//khoi dong gia tri ghi vao DS1307---------------------------------------------------
	tData[0]=Dec2BCD(Second);
	tData[1]=Dec2BCD(Minute);
	
	tData[2]=Dec2BCD(Hour);
/*	tData[3]=Dec2BCD(Day);*/
	tData[4]=Dec2BCD(Date);
	tData[5]=Dec2BCD(Month);
	tData[6]=Dec2BCD(Year);
	TWI_Init(); //khoi dong TWI
	TWI_DS1307_wblock(0x00, tData, 7); //ghi lien tiep cac bien thoi gian vao DS1307
	//----------------------------------------------------------------------------------
	_delay_ms(1);	//cho DS1307 xu li
	Display_7seg(); // hien thi ket qua len LCD
	//doc va hien thi thoi gian lan dau tien******************************************
	TWI_DS1307_wadr(0x00); //set dia chi ve 0
	_delay_ms(1);			//cho DS1307 xu li
	TWI_DS1307_rblock(tData,7); //doc ca khoi thoi gian (7 bytes)
	
	//************************************************************************************

	while(1)
	{
		
	}
	return 0;
}

ISR (TIMER0_OVF_vect )
{
	Time_count++;
	if(Time_count>=10)
	{
		//doc DS1307
		TWI_DS1307_wadr(0x00); 				//set dia chi ve 0
		_delay_ms(1);		   				//cho DS1307 xu li
		TWI_DS1307_rblock(tData,7); 	//doc ca khoi thoi gian (7 bytes)
		//hien thi ket qua len LCD
		if(BCD2Dec(tData[0]) !=Second)
		{ 	//chi hien thi ket qua khi da qua 1s
			Second=BCD2Dec(Second & 0x7F);
//			sprintf(dis, "%i",Second);
//			move_LCD(1,13); print_LCD("  ");
//			move_LCD(1,13); print_LCD(dis);
// 			if (Second==0)
// 				Display_7seg(); 		//moi phut cap nhat 1 lan
		}
		Time_count=0;
	}
}
