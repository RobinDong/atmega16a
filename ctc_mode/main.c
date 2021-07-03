/////////////////////////////////////////
//
// This is an example of electronic organ
// by using CTC mode of timer2.
// Use PORTD 0-6 as buttons for tone.
// Use PORTD 7 to generate wave.
//
// //////////////////////////////////////
#include "avr/io.h"
#include "avr/interrupt.h"

void timer2_init() {
	// Disable all interrupts
	cli();

  TCCR2 = (1 << CS21) | (1 << CS20) | // 1/32 prescaler
          (1 << WGM21); // CTC mode
  TCNT2 = 0;
  OCR2 = 0; // No sound at first
  // Enable overflow interrupt for timer2 on compare match
  TIMSK = (1 << OCIE2);
	// Enable all interrupts.
	sei();
}

int main(void) {
//#### SETUP ####
  // Set PORTA pin 1 as debugger
  DDRA = (1 << DDA1);
  PORTA = (1 << PORTA1);
	// Set PORTD pin 7 (OC2) as output
	DDRD = (1 << DDD7);
  PORTD = 0xff; // Make PORTD pin 0-6 pull up high, PORTD pin 7 output high

  timer2_init();
//#### END OF SETUP ####

  while (1) {
    if ((PIND & (1 << PIND0)) == 0) { // When Button 1 is pressed
      OCR2 = 119; // f= 262Hz
    } else if ((PIND & (1 << PIND1)) == 0) { // When Button 2 is pressed
      OCR2 = 106; // f= 294Hz
    } else if ((PIND & (1 << PIND2)) == 0) { // When Button 3 is pressed
      OCR2 = 96; // f= 327Hz
    } else if ((PIND & (1 << PIND3)) == 0) { // When Button 4 is pressed
      OCR2 = 90; // f= 349Hz
    } else if ((PIND & (1 << PIND4)) == 0) { // When Button 5 is pressed
      OCR2 = 80; // f= 392Hz
    } else if ((PIND & (1 << PIND5)) == 0) { // When Button 6 is pressed
      OCR2 = 72; // f= 436Hz
    } else if ((PIND & (1 << PIND6)) == 0) { // When Button 7 is pressed
      OCR2 = 64; // f= 491Hz
    } else {
      // No sound as default
      TCNT2 = 0;
      OCR2 = 0;
    }
  }
}

ISR(TIMER2_COMP_vect) {
  PORTA ^= (1 << PORTA1); // flip PORTA1 as type of debugger
}
