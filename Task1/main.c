/*
 * Task1.c
 *
 * Created: 16/11/2022 17:09:34
 * Author : Eleanor
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define LED 5
#define BAUDRATE 9600
// System clock speed
#define FOSC 16000000UL
#define BAUDRATE_PRESCALE ( (FOSC / (BAUDRATE * 16UL)) - 1)

// Send a character over USART
void send_USART(unsigned char data)
{
	// Wait for existing data to be send
	while (! ( UCSR0A & (1 << UDRE0) ) )
	{
		// pass;
	}
	// Load data into register to transmit
	UDR0 = data;
}

unsigned char blocking_recieve_USART()
{
	// Wait for indicator of received data
	while (! ( UCSR0A & (1 << RXC0) ) )
	{
		// pass;
	}
	// Return received character
	return UDR0;
}

void init_USART()
{
	// Set USART Baudrate
	UBRR0H = (uint8_t) (BAUDRATE_PRESCALE>>8);
	UBRR0L = (uint8_t) (BAUDRATE_PRESCALE);
	// Enable Rx and Tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Frame format 8 bit data, no parity, 1 stop bit
	UCSR0C = (3 << UCSZ00) | (0 << UPM00) | (0 << USBS0);
}

/*
 * Set up port directions and peripherals, then set initial state
 */
void init()
{
	// Set input/output pins
	DDRB |= (1 << LED);
	// Initialize LED (and all other PORTB pins) to off.
	PORTB = 0x00;
	init_USART();
}

int main(void)
{
	init();
	while (1)
	{
		// Wait for a received command
		unsigned char r = blocking_recieve_USART();
		// Echo command back to PC
		send_USART(r);
		if (r == '1')
		{
			PORTB = (1 << LED);
		}
		else if (r == '0')
		{
			PORTB = (0 << LED);
		}
	}
}

