#include <IO16.h>
#include <SRA16.h>

#include"avr/io.h"
#include"avr/interrupt.h"

#define MOTORR 


//Fixed Parameter
float pi = 3.1415;
volatile long int ticksL = 0;
volatile long int ticksR = 0;
float radius_wheel = 3.3;
float radius_bot = 21.3/2;
int ppr = 60;
int yaw_scaling_factor = 0.5;
int linear_displacement_scaling_factor = 0.5;

//Dynamic Parameters
float number_of_rotations = 0;
float destination_theta = 0,current_theta = 0;
int destination_r = 0,current_r=0;


float yaw_error = 0;
int linear_error = 0;



//float averageEncoderReadings(int ticksLeft, int ticksRight)
//{
// return (ticksLeft + ticksRight) / 2; 
//}

void yaw_correction()
{
  
  if(yaw_error>0)
  {
    bot_spot_left();
  }
  else if(yaw_error<0)
  {
    bot_spot_right();
  }
  
  yaw_value = abs(yaw_error);
  yaw_pwm = map(yaw_value,0,pi,100,400);
   
}

void linear_correction()
{
  linear_value = linear_error;
  linear_pwm = map(linear_value,0,1000,100,400);
}

int main()
{
  Serial.begin(115200);
  MCUCR |= (1 << ISC01);
  GICR |= (1 << INT0) | (1 << INT1);
  DDRD = 0x00;
  PORTD = (1<<2)|(1<<3)|(1<<4)|(1<<5);
  pwm1_init();
  sei();
  destination_theta = atan2(destination_y-initial_y,destination_x-initial_x);
  destination_r = sqrt((destination_y-current_y)*(destination_y-current_y) + (destination_x-current_x)*(destination_x-current_x));

  while (1)
  { 
    number_of_rotations = averageEncoderReadings(ticksL,ticksR)/ppr;
    current_theta = (2*pi*radius_wheel/radius_bot)*number_of_rotations;
    yaw_error = destination_theta - current_theta;
    linear_error = number_of_rotations*2*pi;
    if(-2<yaw_error<2)
    {
      yaw_correction(destination_theta,current_theta); 
    }
    if(-2<linear_error<2)
    {
      linear_correction();
    }
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

/*       number_of_rotations = averageEncoderReadings(ticksL,ticksR)/ppr;
       bot_angle = (2*pi*radius_wheel/radius_bot)*number_of_rotations;
*/

