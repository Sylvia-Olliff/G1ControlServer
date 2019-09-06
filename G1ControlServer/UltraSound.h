#pragma once
#ifndef ULTRASOUND_H
#define ULTRASOUND_H
#include <mutex>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"

class UltraSound {
private:
	std::mutex USMut;
public:
	UltraSound();
	~UltraSound();
	float GetDistance(nThread::CThreadPool&);
};
#endif // !ULTRASOUND_H
