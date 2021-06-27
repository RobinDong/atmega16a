#include "avr/io.h"
#include "avr/wdt.h"
#include "avr/interrupt.h"

unsigned int mode = 0;

void enter_low_power(void) {
  MCUCR = (1 << SE) | (1 << SM1);
  __asm__ __volatile__ ("sleep");
}

int main(void)
{
//#### SETUP ####
	// Disable all interrupts
	cli();
	// Set PORTB pin 0 as output
	DDRB = 0x01;
  PORTB = 0x00;
  // Set all PORTD as input
  DDRD = 0x00;
  // Make PORTD3 pull up high **important**
  PORTD = (1 << PORTD3);

  // Set interrupt for INT1
  MCUCR = (1 << ISC11); // generate interrupt for the falling edge of INT1
  GICR = (1 << INT1);
	// Enable all interrupts.
	sei();
//#### END OF SETUP ####

  while (1) {
    enter_low_power();
  }
}

ISR(INT1_vect) {
  // Flip PORTB0
  PORTB ^= 0x01;
}
