#pragma once
#ifndef LED_H
#define LED_H
#include <mutex>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"

class LED {
private:
	std::mutex LEDMut;
public:
	bool Red;
	bool Green;
	bool Blue;
	LED();
	~LED();
	void SetColor(nThread::CThreadPool&, bool, bool, bool);
};
#endif // !LED_H
