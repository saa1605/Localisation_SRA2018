#include <IO16.h>
#include <SRA16.h>

#include"avr/io.h"
#include"avr/interrupt.h"

#define MOTORR 

float pi = 3.1415;

volatile long int ticksL = 0;
volatile long int ticksR = 0;

float radius_wheel = 3.3;
float radius_bot = 21.3/2;

float number_of_rotations = 0;
float bot_angle = 0;
float dest_angle = 0;
int ppr = 60;

int dest_x = 10;
int dest_y = 10;
int init_x = 0;
int init_y = 0;

float yaw_error = 0;

int yaw_value,yaw_pwm = 0;
int yaw_scaling_factor = 0.5;
int linear_displacement_scaling_factor = 0.5;

float averageEncoderReadings(int ticksLeft, int ticksRight)
{
 return (ticksLeft + ticksRight) / 2; 
}

void yaw_correction(float destination_angle, float initial_angle)
{
  
  yaw_error = degrees(destination_angle - initial_angle);
  if(yaw_error>0)
  {
    bot_spot_left();
  }
  else if(yaw_error<0)
  {
    bot_spot_right();
  }
  yaw_error = abs(yaw_error);
  yaw_value = yaw_error;
  yaw_pwm = map(yaw_value,0,pi,100,400);

  set_pwm1a(yaw_pwm);
  set_pwm1b(yaw_pwm);
   
}

int main()
{
  Serial.begin(115200);
  MCUCR |= (1 << ISC01) ;
  GICR |= (1 << INT0) | (1 << INT1);
  DDRD = 0x00;
  PORTD = (1<<2)|(1<<3)|(1<<4)|(1<<5);
  pwm1_init();
  sei();

  while (1)
  {

    dest_angle = atan2(dest_y-init_y,dest_x-init_x);

    
    while(bot_angle != dest_angle)
    {
      if(dest_angle-bot_angle>0)
      {
        bot_spot_left();
      }
      else
      {
        bot_spot_right();
      }

       number_of_rotations = averageEncoderReadings(ticksL,ticksR)/ppr;
       bot_angle = (2*pi*radius_wheel/radius_bot)*number_of_rotations; 
    }

    init_x = dest_x;
    init_y = dest_y;
  }

}

ISR(INT0_vect)
{
  if (bit_is_set(PIND, 4))
  {
    ticksL++;
  }
  else if (bit_is_clear(PIND, 4))
  {
    ticksL--;
  }
}

ISR(INT1_vect)
{
  if(bit_is_set(PIND,5))
  {
    ticksR++;
  }
  else
  {
    ticksR--;
  }
}

