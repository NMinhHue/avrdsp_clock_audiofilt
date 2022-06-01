/*
 * main.c
 *
 *  Created on: 1 June 2022
 *      Author: Tien_Nguyen
 *      code about single led and button
 */
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU			7372800UL

#define PORT_LED_O      	PORTB
#define DDR_LED_O       	DDRB
#define BIT_LED_O       	6

#define PORT_BUTTON_O      	PORTB
#define DDR_BUTTON_O       	DDRB
#define BIT_BUTTON_O       	7

void Init_IO(void)
{
	// Set LED_BUZ as output pin
	DDR_LED_O |= (1 << BIT_LED_O); //|= la OR (1 << BIT_LED_O) dich trai so bit = voi gia tri BIT_LED_O
	DDR_BUTTON_O &=~ (1 << BIT_BUTTON_O);
	DDR_LED_O &=~ (1 << BIT_LED_O);
	PORTB |= (1 << PB2)|(1 << PB3)|(1 << PB4)|(1 << PB5);
	PORTB |= (1 << PB0)|(1 << PB1);
}

int main(void)
{
	DDRD |= (1 << PD0)|(1 << PD1);	//Output of single_led
	Init_IO();
	// Repeat indefinitely
	for(;;)
	{
		if (bit_is_clear(PINB, 2))
		{
			PORT_LED_O |= (1 << BIT_LED_O);
		}
		if (bit_is_clear(PINB, 3))
		{
			PORT_LED_O |= (1 << BIT_LED_O);
		}
		if (bit_is_clear(PINB, 4))
		{
			PORT_LED_O |= (1 << BIT_LED_O);
		}
		if (bit_is_clear(PINB, 5))
		{
			PORT_LED_O &= ~(1 << BIT_LED_O);
		}
		// Disable LED_BUZ
		_delay_ms(500);
	}
}

