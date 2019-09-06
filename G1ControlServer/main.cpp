#include <wiringPi.h>
#include <softPwm.h>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"
#include "Camera.h"
#include "FrontServo.h"
#include "Drive.h"
#include "Pins.h"
#include "LED.h"
#include "UltraSound.h"
#include "Server.h"


int main(void)
{
	wiringPiSetup();

	nThread::CThreadPool TPool{ 6 };

	Camera Cam = Camera(TPool);
	FrontServo FS = FrontServo(TPool);
	Drive Engine = Drive();
	LED Rgb = LED();
	UltraSound US = UltraSound();
	Server Srv = Server();

	Srv.Listen();

	/*Rgb.SetColor(TPool, false, false, true);
	Cam.CameraLeft(TPool, 60);
	delay(2000);
	Rgb.SetColor(TPool, false, true, false);
	Cam.CameraRight(TPool, 120);
	delay(2000);
	Rgb.SetColor(TPool, true, false, false);
	Cam.CameraLeft(TPool, 60);
	delay(2000);*/

	TPool.join_all();
	
	return 0;
}