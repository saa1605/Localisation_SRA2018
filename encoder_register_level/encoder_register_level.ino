#include"avr/io.h"
#include"avr/interrupt.h"


volatile long int ticks = 0;


int main()
{


  Serial.begin(115200);
  MCUCR |= (1 << ISC01) ;
  GICR |= (1 << INT0) ;
  sei();

  while (1)
  {

      Serial.println(ticks);

  }

}

ISR(INT1_vect)
{
  if (bit_is_set(PIND, 4))
  {
    ticks++;
  }
  else if (bit_is_clear(PIND, 4))
  {
    ticks--;
  }
}

//ISR(INT0_vect)
//{
//  ticks = 0;
//}


