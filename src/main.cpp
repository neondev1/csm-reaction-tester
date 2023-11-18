#include <random>
#include "main.h"

using namespace pros;

const char* prompts[] = {
	"Drive forwards         ",
	"Drive backwards        ",
	"Turn left              ",
	"Turn right             ",
	"Set forwards direction ",
	"Set backwards direction",
	0,
	0,
	"Open/close wall        ",
	0,
	"Raise/lower intake     ",
	0,
	"Intake                 ",
	"Outtake                ",
	"Toggle catapult        ",
	"Catapult up/down       "
};

Controller master(E_CONTROLLER_MASTER);

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
	int correct = 0, incorrect = 0;
	int last = 0, fastest = 2147483647, slowest = 0;
	double mean = 0.0;
	srand(time(__null));
	std::mt19937 generator(rand());
	std::uniform_int_distribution<> rand(0, 15);
	screen::print(E_TEXT_MEDIUM, 0, "");
	delay(1000);
	screen::print(E_TEXT_MEDIUM, 0, "CSM REACTION TIME TESTER v1.0");
	screen::print(E_TEXT_LARGE, 1, "3");
	screen::print(E_TEXT_MEDIUM, 3, "Last:");
	screen::print(E_TEXT_MEDIUM, 4, "Mean:");
	screen::print(E_TEXT_MEDIUM, 5, "Fastest:");
	screen::print(E_TEXT_MEDIUM, 6, "Slowest:");
	screen::print(E_TEXT_MEDIUM, 7, "Correct:");
	screen::print(E_TEXT_MEDIUM, 8, "Incorrect:");
	delay(1000);
	screen::print(E_TEXT_LARGE, 1, "2");
	delay(1000);
	screen::print(E_TEXT_LARGE, 1, "1");
	delay(1000);
	int time = 0;
	for (;; delay(1000)) {
		int num = rand(generator);
		if (num == 6 || num == 7 || num == 9 || num == 11)
			continue;
		screen::print(E_TEXT_LARGE, 1, prompts[num]);
		time = millis();
		for (;; delay(13)) {
			short expected = 0;
			expected |= (1 << num);
			short mask = 0;
			mask |=
				  ((master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)  >  8)	<< 0)
				| ((master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)  < -8)	<< 1)
				| ((master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) < -8)	<< 2)
				| ((master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) >  8)	<< 3)
				| (master.get_digital(E_CONTROLLER_DIGITAL_UP) 				<< 4)
				| (master.get_digital(E_CONTROLLER_DIGITAL_DOWN)			<< 5)
				| (master.get_digital(E_CONTROLLER_DIGITAL_LEFT)			<< 6)
				| (master.get_digital(E_CONTROLLER_DIGITAL_RIGHT)			<< 7)
				| (master.get_digital(E_CONTROLLER_DIGITAL_X)				<< 8)
				| (master.get_digital(E_CONTROLLER_DIGITAL_Y)				<< 9)
				| (master.get_digital(E_CONTROLLER_DIGITAL_A)				<< 10)
				| (master.get_digital(E_CONTROLLER_DIGITAL_B)				<< 11)
				| (master.get_digital(E_CONTROLLER_DIGITAL_L1)				<< 12)
				| (master.get_digital(E_CONTROLLER_DIGITAL_L2)				<< 13)
				| (master.get_digital(E_CONTROLLER_DIGITAL_R1)				<< 14)
				| (master.get_digital(E_CONTROLLER_DIGITAL_R2)				<< 15);
			if (mask == 0)
				continue;
			if (expected != mask) {
				incorrect++;
				screen::print(E_TEXT_LARGE, 1, "Incorrect              ");
				screen::print(E_TEXT_MEDIUM, 8, "Incorrect: %d", incorrect);
				break;
			}
			correct++;
			last = millis() - time;
			mean = (mean * ((double)correct - 1.0) + (double)last) / (double)correct;
			if (last > slowest)
				slowest = last;
			if (last < fastest)
				fastest = last;
			screen::print(E_TEXT_LARGE, 1, "Correct!               ");
			screen::print(E_TEXT_MEDIUM, 3, "Last:    %9d ms", last);
			screen::print(E_TEXT_MEDIUM, 4, "Mean:   %9f ms  ", mean);
			screen::print(E_TEXT_MEDIUM, 5, "Fastest: %9d ms", fastest);
			screen::print(E_TEXT_MEDIUM, 6, "Slowest: %9d ms", slowest);
			screen::print(E_TEXT_MEDIUM, 7, "Correct:   %d", correct);
			break;
		}
	}
}
