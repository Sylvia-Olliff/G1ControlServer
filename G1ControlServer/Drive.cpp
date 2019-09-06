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

void Drive::Forward(nThread::CThreadPool& TPool, unsigned int speed)
{
	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed, true, false, speed);

	CurrentSpeed = speed;
	Status = MovingForward;
	return;
}

void Drive::Reverse(nThread::CThreadPool& TPool, unsigned int speed)
{
	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, speed, false, true, speed);

	CurrentSpeed = speed;
	Status = MovingReverse;
	return;
}

void Drive::TurnLeft(nThread::CThreadPool& TPool, unsigned int speed)
{
	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed / 10, true, false, speed);

	CurrentSpeed = speed;
	Status = TurningLeft;
	return;
}

void Drive::TurnRight(nThread::CThreadPool& TPool, unsigned int speed)
{
	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed, true, false, speed / 10);

	CurrentSpeed = speed;
	Status = TurningRight;
	return;
}

void Drive::SpinLeft(nThread::CThreadPool& TPool, unsigned int speed)
{
	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, true, speed, true, false, speed);

	CurrentSpeed = speed;
	Status = SpinningLeft;
	return;
}

void Drive::SpinRight(nThread::CThreadPool& TPool, unsigned int speed)
{
	speed = CheckSpeed(speed);

	TPool.add_and_detach(Commands::DriveCommand, DriveMut, true, false, speed, false, true, speed);

	CurrentSpeed = speed;
	Status = SpinningRight;
	return;
}

void Drive::Brake(nThread::CThreadPool& TPool)
{
	TPool.add_and_detach(Commands::DriveCommand, DriveMut, false, false, MIN_PWM, false, false, MIN_PWM);

	CurrentSpeed = MIN_PWM;
	Status = Stopped;
	return;
}


