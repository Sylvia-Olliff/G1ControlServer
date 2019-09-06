#include <wiringPi.h>
#include <softPwm.h>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"
#include "Camera.h"
#include "FrontServo.h"
#include "Drive.h"
#include "Pins.h"
#include "LED.h"
#include "UltraSound.h"


int main(void)
{
	wiringPiSetup();

	nThread::CThreadPool TPool{ 6 };

	Camera Cam = Camera(TPool);
	FrontServo FS = FrontServo(TPool);
	Drive Engine = Drive();
	LED Rgb = LED();
	UltraSound US = UltraSound();

	
	TPool.join_all();
	
	return 0;
}