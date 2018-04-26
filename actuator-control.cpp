#include <cstdint>
//#include <cunistd>
#include <iostream>
#include <cmath>
#include <chrono>
extern "C" {
	#include <roboticscape.h>
	#include <rc_usefulincludes.h>
}


#define ENA 1   // servo output 1
#define IN1 113 // gpio pin 3*32 + 17 = 113
#define IN2 116 // gpio pin 3*32 + 20 = 116

// forward declaration
void motor_pulse(int servo, double pulsewidth);

int main()
{
	rc_initialize();

	rc_set_default_pinmux();
	rc_gpio_export(IN1);
	rc_gpio_set_dir(IN1, OUTPUT_PIN);
	rc_gpio_export(IN2);
	rc_gpio_set_dir(IN2, OUTPUT_PIN);

	// Basically this program makes the linear actuator oscillate
	for (int i = 0; i < 40; ++i) {
		std::cout << "Iteration no. " << i << std::endl;

		// vary this throughout the program to change the PWM length
		double duty = 0.99;
		//direction of the pwm trend: true => going down, false => going up
		bool dir = true;
		while (duty <= 1.0) {
			motor_pulse(ENA, duty);
			duty = (dir)? duty-0.01 : duty+0.01;
			if (duty <= -1.0)
				dir = false;
		}
	}

}

// motor_pulse
// Pulse the motor with PWM fraction of pulsewidth, with -1.0 <= pulsewidth <= 1.0
// assume that "servo" has already been properly initialized
void motor_pulse(int servo, double pulsewidth) {
	// each pulse cycle is 0.1s, or 100,000us.
	static int TOTAL_DUR_US = 100000;

	if (pulsewidth < 0) {
		pulsewidth *= -1.0;
		rc_gpio_set_value(IN2, HIGH);
		rc_gpio_set_value(IN1, LOW);
	} else {
		rc_gpio_set_value(IN1, HIGH);
		rc_gpio_set_value(IN2, LOW);
	}

	int high_time = int(pulsewidth * double(TOTAL_DUR_US));

	// send the pulse!
	rc_send_servo_pulse_us(servo, high_time);

	//std::chrono::time_point start = std::chrono::system_clock::now();
	auto start = std::chrono::system_clock::now();

	while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() < high_time) // sorry
	{/* wait until the servo is done pulsing */}
	// signal wire is low now, so delay for the remainder of the cycle
	while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() < TOTAL_DUR_US) // sorry again
	{ /* total time elapsed = 10,000us */ }
}
