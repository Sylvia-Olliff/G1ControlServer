#pragma once
#ifndef FRONTSERVO_H
#define FRONTSERVO_H
#include <mutex>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"
class FrontServo {
private:
	std::mutex FSMut;
public:
	int FrontServoPOS;
	FrontServo(nThread::CThreadPool&);
	~FrontServo();
	void ServoLeft(nThread::CThreadPool&, int);
	void ServoRight(nThread::CThreadPool&, int);
	void ServoScan(nThread::CThreadPool&);
};
#endif // !FRONTSERVO_H
