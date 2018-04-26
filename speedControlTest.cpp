#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
extern "C" {
	#include <roboticscape.h>
	#include <rc_usefulincludes.h>
}

int main()
{
	rc_initialize();
	unsigned int ENA = 0; //channel 8??
	unsigned int IN1 = 0; //gpio pin value
	unsigned int IN2 = 0; //gpio pin value
	double time = 0; //put 1/4 time???

	rc_gpio_export(IN1);
	rc_gpio_set_dir(IN1);
	rc_gpio_export(IN2);
	rc_gpio_set_dir(IN2);

	while(1)
	{
		motor1_forward(ENA, time);
	}

}

void motor1_forward(int ch, int us)
{
	rc_gpio_set_value(IN1, HIGH);
	rc_gpio_set_value(IN2, HIGH);
	rc_send_servo_pulse_us(ch, us);
}
