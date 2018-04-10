#include <IO16.h>
#include <SRA16.h>

#include"avr/io.h"
#include"avr/interrupt.h"

//Fixed Parameter
float pi = 3.1415;
volatile long int ticksL = 0;
volatile long int ticksR = 0;
float radius_wheel = 3.3;
float radius_bot = 21.3/2;
int ppr = 60;
int yaw_scaling_factor = 0.7;
int linear_displacement_scaling_factor = 0.3;
float current_theta = 0;
float destination_theta = 0;
float number_of_rotations = 0;
int destination_displacement = 0;
int linear_factor = 0;
float linear_displacement = 0;
int yaw_error = 0;
int pwml = 0;
int pwmr = 0;
int error = 0;
int opt = 350;
//float error_correction()
//{
//  
//}


int main()
{
  
  Serial.begin(115200);
  DDRD = 0x00;
  DDRC = 0xff;
  PORTD |=(1<<6)|(1<<2)|(1<<7)|(1<<3);
  MCUCR |= (1<<ISC01)|(1<<ISC11);
  GICR |= (1 << INT0)|(1<<INT1) ;
  sei();
  pwm1_init();
  sei();
//  destination_r = sqrt((destination_y-current_y)*(destination_y-current_y) + (destination_x-current_x)*(destination_x-current_x));
  destination_theta = 135;
  while(1)
  {
    if(destination_theta - current_theta > 0)
    {
      bot_spot_left();
    }
    else
    {
      bot_spot_right();
    }

    if(abs(destination_theta - current_theta) <5)
    {
      bot_stop();
    }

    number_of_rotations = abs(ticksL-ticksR)/ppr;
    current_theta = (2*pi*radius_wheel/radius_bot)*number_of_rotations;
    yaw_error = abs(destination_theta - degrees(current_theta));

    set_pwm1a(opt + yaw_error);
    set_pwm1b(opt + yaw_error);

  }
  
}



ISR(INT0_vect)
{
  if (bit_is_set(PIND, 6))
  {
    ticksR--;
  }
  else if (bit_is_clear(PIND, 6))
  {
    ticksR++;
  }
}

ISR(INT1_vect)
{
  if(bit_is_set(PIND,7))
  {
    ticksL++;
  }
  else if(bit_is_clear(PIND,7))
  {
    ticksL--;
  }
}
