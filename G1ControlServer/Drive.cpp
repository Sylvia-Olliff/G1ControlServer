#include <wiringPi.h>
#include <softPwm.h>
#include "Drive.h"
#include "Pins.h"
#include "Commands.cpp"
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"

unsigned int Drive::CheckSpeed(unsigned int speed)
{
	if (speed > MAX_PWM)
	{
		speed = MAX_PWM;
		return speed;
	}
	return speed;
}

Drive::Drive()
{
	pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
	pinMode(MOTOR_LEFT_BACK, OUTPUT);
	pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
	pinMode(MOTOR_RIGHT_BACK, OUTPUT);

	pinMode(TRACK_SENSOR_LEFT_1, INPUT);
	pinMode(TRACK_SENSOR_LEFT_2, INPUT);
	pinMode(TRACK_SENSOR_RIGHT_1, INPUT);
	pinMode(TRACK_SENSOR_RIGHT_2, INPUT);

	softPwmCreate(MOTOR_LEFT_PWM, MIN_PWM, MAX_PWM);
	softPwmCreate(MOTOR_RIGHT_PWM, MIN_PWM, MAX_PWM);

	Status = Stopped;
	CurrentSpeed = MIN_PWM;
}

Drive::~Drive()
{
	pinMode(MOTOR_LEFT_FORWARD, INPUT);
	pinMode(MOTOR_LEFT_BACK, INPUT);
	pinMode(MOTOR_RIGHT_FORWARD, INPUT);
	pinMode(MOTOR_RIGHT_BACK, INPUT);

	softPwmWrite(MOTOR_LEFT_PWM, MIN_PWM);
	softPwmWrite(MOTOR_RIGHT_PWM, MIN_PWM);
	softPwmStop(MOTOR_LEFT_PWM);
	softPwmStop(MOTOR_RIGHT_PWM);
}


TreadStatus Drive::GetStatus()
{
	return Status;
}

void Drive::EnableTracking() {
	Tracking = true;
	Status = TreadStatus::Tracking;
	return;
}

void Drive::DisableTracking() {
	Tracking = false;
	return;
}

void Drive::Track(nThread::CThreadPool& TPool)
{
	int TrackSensorLeftValue1;
	int TrackSensorLeftValue2;
	int TrackSensorRightValue1;
	int TrackSensorRightValue2;

	while (Tracking) 
	{
		TrackSensorLeftValue1 = digitalRead(TRACK_SENSOR_LEFT_1);
		TrackSensorLeftValue2 = digitalRead(TRACK_SENSOR_LEFT_2);
		TrackSensorRightValue1 = digitalRead(TRACK_SENSOR_RIGHT_1);
		TrackSensorRightValue2 = digitalRead(TRACK_SENSOR_RIGHT_2);

		if ((TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) && TrackSensorRightValue2 == LOW) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, TRACKING_SPEED, false, true, TRACKING_SPEED);
			delay(80);
		}
		else if (TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW || TrackSensorRightValue2 == LOW)) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, TRACKING_SPEED, true, false, TRACKING_SPEED);
			delay(80);
		}
		else if (TrackSensorLeftValue1 == LOW) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, TRACKING_SPEED_LOW, true, false, TRACKING_SPEED_LOW);
		}
		else if (TrackSensorRightValue2 == LOW) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, TRACKING_SPEED_LOW, false, true, TRACKING_SPEED_LOW);
		}
		else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, TRACKING_SPEED_LOW, true, false, TRACKING_SPEED_LOW);
		}
		else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, TRACKING_SPEED_LOW, false, true, TRACKING_SPEED_LOW);
		}
		else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, TRACKING_SPEED, true, false, TRACKING_SPEED);
		}
		else if (TrackSensorLeftValue1 == HIGH && TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == HIGH && TrackSensorRightValue2 == HIGH) {
			TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, TRACKING_SPEED_LOW, false, true, TRACKING_SPEED_LOW);
		}
	}
	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, false, MIN_PWM, false, false, MIN_PWM);
	Status = Stopped;
	return;
}

void Drive::Forward(nThread::CThreadPool& TPool, unsigned int speed)
{
	if (Tracking)
		return;

	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed, true, false, speed);

	CurrentSpeed = speed;
	Status = MovingForward;
	return;
}

void Drive::Reverse(nThread::CThreadPool& TPool, unsigned int speed)
{
	if (Tracking)
		return;

	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, speed, false, true, speed);

	CurrentSpeed = speed;
	Status = MovingReverse;
	return;
}

void Drive::TurnLeft(nThread::CThreadPool& TPool, unsigned int speed)
{
	if (Tracking)
		return;

	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed / 10, true, false, speed);

	CurrentSpeed = speed;
	Status = TurningLeft;
	return;
}

void Drive::TurnRight(nThread::CThreadPool& TPool, unsigned int speed)
{
	if (Tracking)
		return;

	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed, true, false, speed / 10);

	CurrentSpeed = speed;
	Status = TurningRight;
	return;
}

void Drive::SpinLeft(nThread::CThreadPool& TPool, unsigned int speed)
{
	if (Tracking)
		return;

	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, speed, true, false, speed);

	CurrentSpeed = speed;
	Status = SpinningLeft;
	return;
}

void Drive::SpinRight(nThread::CThreadPool& TPool, unsigned int speed)
{
	if (Tracking)
		return;

	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed, false, true, speed);

	CurrentSpeed = speed;
	Status = SpinningRight;
	return;
}

void Drive::Brake(nThread::CThreadPool& TPool)
{
	if (Tracking)
		return;

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, false, MIN_PWM, false, false, MIN_PWM);

	CurrentSpeed = MIN_PWM;
	Status = Stopped;
	return;
}


