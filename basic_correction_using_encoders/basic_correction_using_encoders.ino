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

int current_x = 0;
int current_y = 0;

int destination_x = 100;
int destination_y = 100;

//Dynamic Parameters
float number_of_rotations = 0;
float destination_theta = 0,current_theta = 0;
int destination_r = 0,current_r=0;
int left_control_input = 0,right_control_input = 0;

float yaw_error = 0;
int linear_error = 0;

float averageEncoderReadings(int ticksLeft, int ticksRight)
{
 return (ticksLeft + ticksRight) / 2; 
}

int yaw_correction(float yaw_error)
{  
  int yaw_pwm = 0;
  yaw_pwm = degrees(yaw_error);
  return yaw_pwm;  
}

int linear_correction(int linear_error)
{
  int linear_value = linear_error;
  int linear_pwm = map(linear_value,0,100,100,400);
  return linear_pwm;
}

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
  destination_theta = atan2(destination_y-current_y,destination_x-current_x);
  destination_r = sqrt((destination_y-current_y)*(destination_y-current_y) + (destination_x-current_x)*(destination_x-current_x));

  while (1)
  { 
    number_of_rotations = averageEncoderReadings(ticksL,ticksR)/ppr;
    current_theta = (2*pi*radius_wheel/radius_bot)*number_of_rotations;
    yaw_error = destination_theta - current_theta;
    linear_error = destination_r-number_of_rotations*2*pi*radius_wheel;
    if(abs(linear_error)>4)
    {
      left_control_input_linear = linear_displacement_scaling_factor*linear_correction(linear_error);
      right_control_input_linear = linear_displacement_scaling_factor*linear_correction(linear_error);
    }
    if(abs(yaw_error)>4)
    {
      left_control_input_yaw = yaw_scaling_factor*yaw_correction(yaw_error);
      right_control_input_yaw = -yaw_scaling_factor*yaw_correction(yaw_error);
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

