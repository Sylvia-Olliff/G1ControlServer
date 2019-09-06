#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <mutex>
#include <chrono>
#include "Commands.h"
#include "Pins.h"



static void Commands::ServoPulse(std::mutex& mut, int pin, int angle) {
	int pulseWidth;
	pulseWidth = (angle * 11) + 500;
	std::lock_guard<std::mutex> lock{ mut };
	digitalWrite(pin, HIGH);
	delayMicroseconds(pulseWidth);
	digitalWrite(pin, LOW);
	delay(20 - pulseWidth / 1000); // Smooths out the movement
	return;
}

static void Commands::DriveCommand(std::mutex& mut, bool leftForward, bool leftBack, int leftSpeed, bool rightForward, bool rightBack, int rightSpeed) {
	std::lock_guard<std::mutex> lock{ mut };
	digitalWrite(MOTOR_LEFT_FORWARD, (leftForward) ? HIGH : LOW);
	digitalWrite(MOTOR_LEFT_BACK, (leftBack) ? HIGH : LOW);
	softPwmWrite(MOTOR_LEFT_PWM, leftSpeed);

	digitalWrite(MOTOR_RIGHT_FORWARD, (rightForward) ? HIGH : LOW);
	digitalWrite(MOTOR_RIGHT_BACK, (rightBack) ? HIGH : LOW);
	softPwmWrite(MOTOR_RIGHT_PWM, rightSpeed);
	return;
}

static void Commands::ServoScan(std::mutex& mut, int pin) {
	int pos1 = 0;
	int pulseWidth;
	std::lock_guard<std::mutex> lock{ mut };
	for (pos1 = 0; pos1 < 180; pos1 += 2) {
		Commands::ServoPulse(mut, pin, pos1);
		delay(20);
	}
	int pos2 = 180;
	for (pos2 = 180; pos2 > 0; pos2 -= 2) {
		Commands::ServoPulse(mut, pin, pos2);
		delay(20);
	}
	return;
}

static void Commands::SetPin(std::mutex& mut, int pin, bool condition) {
	std::lock_guard<std::mutex> lock{ mut };
	digitalWrite(pin, (condition) ? HIGH : LOW);
	return;
}

static void Commands::SetColor(std::mutex& mut, bool red, bool green, bool blue) {
	std::lock_guard<std::mutex> lock{ mut };
	digitalWrite(LED_R, (red) ? HIGH : LOW);
	digitalWrite(LED_G, (green) ? HIGH : LOW);
	digitalWrite(LED_B, (blue) ? HIGH : LOW);
	return;
}

static float Commands::Distance() {
	float distance;
	long start, stop;

	digitalWrite(US_PING, LOW);
	delayMicroseconds(2);
	digitalWrite(US_PING, HIGH);
	delayMicroseconds(15);
	digitalWrite(US_PING, LOW);

	start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	while (!digitalRead(US_ECHO) == 1) {
		stop = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		if ((stop - start) > 30000)
			return -1;
	}

	start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	
	while (!digitalRead(US_ECHO) == 0) {
		stop = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		if ((stop - start) > 30000)
			return -1;
	}

	stop = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	distance = (float)(stop - start) / 1000000 * 34000 / 2;
	return distance;
}

static void Commands::GetDistance(std::mutex& mut, float& distance) {
	std::lock_guard<std::mutex> lock{ mut };
	unsigned long ultrasonic[5] = { 0 };
	int num = 0;

	while (num < 5)
	{
		distance = Commands::Distance();
		while ((int)distance == -1)
		{
			distance = Commands::Distance();
		}

		while ((int)distance >= 500 || (int)distance == 0) 
		{
			distance = Commands::Distance();
		}
		ultrasonic[num] = distance;
		num++;
		delay(10);
	}
	num = 0;
	Commands::bubble(ultrasonic, 5);
	distance = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;

	return;
}

static void Commands::bubble(unsigned long* a, int n) {
	int i, j, temp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (a[i] > a[j])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
}

