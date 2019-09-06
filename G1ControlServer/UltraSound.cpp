#include <wiringPi.h>
#include "Pins.h"
#include "UltraSound.h"
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"
#include "Commands.cpp"

float UltraSound::GetDistance(nThread::CThreadPool& TPool)
{
	float distance;
	size_t id = TPool.add(Commands::GetDistance, USMut, distance);
	TPool.join(id);
	return distance;
}

UltraSound::UltraSound() {
	pinMode(US_ECHO, INPUT);
	pinMode(US_PING, OUTPUT);
}

UltraSound::~UltraSound() {
	pinMode(US_ECHO, OUTPUT);
	pinMode(US_PING, INPUT);
}


