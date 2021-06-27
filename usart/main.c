////////////////////////////////////////////////////////////
// Remember to set serial port by using:
//    stty -f /dev/tty.usbserial-144120 cs8 -parenb -cstopb
// on MacOS and then:
//    screen /dev/tty.usbserial-144120 4800
//
// We can't enter power-save mode for USART because it need
// CPU-clock
////////////////////////////////////////////////////////////
#include <string.h>
#include "avr/io.h"
#include "avr/wdt.h"
#include "avr/interrupt.h"

#define BAUD 4800
#define MYUBRR F_CPU/16/BAUD-1

unsigned int pos = 0;
const char *message = "Hello world!\n\r";

void usart_init(unsigned int ubrr) {
  // Set baud rate
  UBRRH = (unsigned char)(ubrr >> 8);
  UBRRL = (unsigned char)ubrr;
  // Enable transmitter
  UCSRB = (1 << TXEN) | (1 << RXEN);
  // Set frame format: 8data, 1stop bit, no parity
  UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

void usart_transmit(unsigned char data) {
  // Wait for empty transmit buffer
  while (!(UCSRA & (1 << UDRE)));
  // Put data into buffer, sends the data
  UDR = data;
}

int main(void)
{
//#### SETUP ####
	// Disable all interrupts
	cli();
	// Set  PORTB pin 0 as output, we use this pin like debugger
	DDRB = 0x01;
  PORTB = 0x00;

  TCCR2=(1 << CS20) | (1 << CS21) | (1 << CS22);
  TCNT2=0x00;
  TIMSK=(1 << TOIE2);

  usart_init(MYUBRR);
	// Enable all interrupts.
	sei();
//#### END OF SETUP ####

  while (1) {
  }
}

ISR(TIMER2_OVF_vect) {
  // Flip bit 3 of PORTB
  PORTB ^= 0x01;

  // Send message character by character through USART
  usart_transmit(message[pos++]);
  if (pos >= strlen(message)) {
    pos = 0;
  }

  TCNT2 = 0x00;
	sei();
}
