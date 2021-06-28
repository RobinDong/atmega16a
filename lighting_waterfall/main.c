//////////////////////////////////////////
// Use port PAx and PCx to light the LEDs
//////////////////////////////////////////
#include "avr/io.h"
#include "avr/wdt.h"
#include "avr/interrupt.h"

volatile unsigned int mode = 0;
volatile unsigned short ports_map = 1;

void enter_low_power(void) {
  MCUCR = (1 << SE) | (1 << SM0);
  __asm__ __volatile__ ("sleep");
}

// light from PC0->PC7 and PA7->PA0
void upside(void) {
  unsigned char high = (ports_map >> 8);
  if (high == 0) {
    PORTA = high;
  } else {
    PORTA = (128 / high);
  }
  unsigned char low = 0x00FF & ports_map;
  PORTC = low;
  ports_map <<= 1;
  if (ports_map == 0) {
    ports_map = 1;
  }
}

// light from PA0->PA7 and PC7->PC0
void downside(void) {
  unsigned char high = (ports_map >> 8);
  if (high == 0) {
    PORTA = high;
  } else {
    PORTA = (128 / high);
  }
  unsigned char low = 0x00FF & ports_map;
  PORTC = low;
  ports_map >>= 1;
  if (ports_map == 0) {
    ports_map = 0x8000;
  }
}

int main(void)
{
//#### SETUP ####
	// Disable all interrupts
	cli();
	// Set PORTB pin 0 as output
	DDRB = 0x01;
  PORTB = 0x00;
  // Set PORTA all as output
  DDRA = 0xFF;
  PORTA = 0x00;
  // Set PORTC all as output
  DDRC = 0xFF;
  PORTC = 0x00;
  // Set all PORTD as input
  DDRD = 0x00;
  // Make PORTD3 pull up high **important**
  PORTD = (1 << PORTD3);

  // Set interrupt for INT1
  MCUCR = (1 << ISC11); // generate interrupt for the falling edge of INT1
  GICR = (1 << INT1);

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

  TCNT2 = 0xC0;

  // Run the lighting waterfall by the mode
  if (mode == 0) {
    upside();
  } else if (mode == 1) {
    downside();
  }
}

ISR(INT1_vect) {
  // Flip PORTB0
  PORTB ^= 0x01;
  // `mode` loops in range [0, 1]
  if (++mode > 1) {
    mode = 0;
  }
}
