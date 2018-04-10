#include"avr/io.h"
#include"avr/interrupt.h"


volatile long int ticksL = 0;
volatile long int ticksR = 0;

int main()
{


  Serial.begin(115200);
  DDRD = 0x00;
  PORTD |=(1<<6)|(1<<2)|(1<<7)|(1<<3);
  MCUCR |= (1<<ISC01)|(1<<ISC11);
  GICR |= (1 << INT0)|(1<<INT1) ;
  sei();

  while (1)
  {
      Serial.print("left:");Serial.println(ticksL);
      Serial.print("right:");Serial.println(ticksR);
  }

}

ISR(INT0_vect)
{
  if (bit_is_set(PIND, 6))
  {
    ticksL++;
  }
  else if (bit_is_clear(PIND, 6))
  {
    ticksL--;
  }
}

ISR(INT1_vect)
{
  if (bit_is_set(PIND, 7))
  {
    ticksR++;
  }
  else if (bit_is_clear(PIND, 7))
  {
    ticksR--;
  }
}


