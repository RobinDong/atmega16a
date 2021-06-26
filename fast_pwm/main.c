#include "avr/io.h"
#include "avr/interrupt.h"

unsigned int timeout = 0;
unsigned int direction = 0;

void enter_low_power(void) {
  MCUCR = 0x00 | (1 << SE) | (1 << SM0);
  __asm__ __volatile__ ("sleep");
}

int main(void)
{
//#### SETUP ####
	// Disable all interrupts
	cli();
  // Set PORTB pin 0 as output (we use it like a debugger)
  DDRB = (1 << DDB0);
  PORTB = (1 << PORTB0);
	// Set  PORTD pin 7 (OC2) as output
	DDRD = (1 << DDD7);
  PORTD = (1 << PORTD7);

  TCCR2=(1 << CS20) | // no prescaler
        (1 << WGM20) | (1 << WGM21) | // Fast PWM mode
        (1 << COM21); // clear OC2 on compare match
  OCR2=0xFF; // Let LED (OC2) shine

  // Enable overflow interrupt for timer2
  TIMSK=(1 << TOIE2);
	// Enable all interrupts.
	sei();
//#### END OF SETUP ####

  while (1) {
    enter_low_power();
  }
}

ISR(TIMER2_OVF_vect) {
  if (++timeout > 500) {
    timeout = 0;
    // Flip PORTB0
    PORTB ^= 0x01;
    if (direction) { // make the LED in OC2 lighter
      OCR2 = (OCR2 << 1) + 1;
      if (OCR2 >= 0xFF) {
        direction = 0;
      }
    } else { // make the LED in OC2 dimmer
      OCR2 = OCR2 >> 1;
      if (OCR2 <= 0) {
        direction = 1;
      }
    }
  }
	sei();
}
