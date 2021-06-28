#include "avr/io.h"
#include "avr/wdt.h"
#include "avr/interrupt.h"

unsigned int timeout = 0;

void enter_low_power(void) {
  MCUCR = (1 << SE) | (1 << SM0);
  __asm__ __volatile__ ("sleep");
}

int main(void)
{
//#### SETUP ####
	// Disable all interrupts
	cli();
	// Set  PORTB pin 0 as output
	DDRB = 0x01;
  PORTB = 0x00;

  // Setup timer2
  TCCR2=(1 << CS20) | (1 << CS21) | (1 << CS22);
  TCNT2=0x00;
  TIMSK=(1 << TOIE2);
	// Enable all interrupts.
	sei();
//#### END OF SETUP ####

  while (1) {
    enter_low_power();
  }
}

ISR(TIMER2_OVF_vect) {
  // Flip bit 3 of PORTB
  PORTB ^= 0x01;

  TCNT2 = 0x00;
}
