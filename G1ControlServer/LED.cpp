#include <wiringPi.h>
#include "LED.h"
#include "Commands.cpp"
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"

LED::LED() {
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
}

LED::~LED() {
	Commands::SetColor(LEDMut, false, false, false);
}

void LED::SetColor(nThread::CThreadPool& TPool, bool red, bool blue, bool green)
{
	Red = red;
	Blue = blue;
	Green = green;

	TPool.add_and_detach(Commands::SetColor, LEDMut, red, blue, green);
	return;
}
