#include <wiringPi.h>
#include <mutex>
#include "FrontServo.h"
#include "Pins.h"
#include "Commands.cpp"
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"


	FrontServo::FrontServo(nThread::CThreadPool& TPool)
	{
		pinMode(FRONT_SERVO, OUTPUT);
		TPool.add_and_detach(Commands::ServoPulse, FSMut, FRONT_SERVO, 90);
		TPool.join_all();
		FrontServoPOS = 90;
	}

	FrontServo::~FrontServo()
	{
		pinMode(FRONT_SERVO, INPUT);
	}


	void FrontServo::ServoLeft(nThread::CThreadPool& TPool, int angleChange)
	{
		int newAngle = FrontServoPOS - angleChange;
		if (newAngle < 0)
			newAngle = 0;

		TPool.add_and_detach(Commands::ServoPulse, FSMut, FRONT_SERVO, newAngle);
		delay(20);
		return;
	}

	void FrontServo::ServoRight(nThread::CThreadPool& TPool, int angleChange)
	{
		int newAngle = FrontServoPOS + angleChange;
		if (newAngle > 180)
			newAngle = 180;

		TPool.add_and_detach(Commands::ServoPulse, FSMut, FRONT_SERVO, newAngle);
		delay(20);
		return;
	}

	void FrontServo::ServoScan(nThread::CThreadPool& TPool)
	{
		TPool.add_and_detach(Commands::ServoScan, FSMut, FRONT_SERVO);
		return;
	}

