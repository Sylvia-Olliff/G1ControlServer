#pragma once
#ifndef DRIVE_H
#define DRIVE_H
#include <mutex>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"
enum TreadStatus
{
	MovingForward,
	MovingReverse,
	Stopped,
	TurningLeft,
	TurningRight,
	SpinningLeft,
	SpinningRight,
	Tracking
};

constexpr unsigned int MAX_PWM = 255;
constexpr unsigned int MIN_PWM = 0;
constexpr unsigned int DEF_SPEED = 125;
constexpr unsigned int TRACKING_SPEED = 145;
constexpr unsigned int TRACKING_SPEED_LOW = 110;

class Drive {
private:
	std::mutex DriveMut;
	unsigned int CurrentSpeed;
	TreadStatus Status;
	unsigned int CheckSpeed(unsigned int);
	bool Tracking;
public:
	Drive();
	~Drive();
	TreadStatus GetStatus();
	void EnableTracking();
	void DisableTracking();
	void Track(nThread::CThreadPool&);
	void Forward(nThread::CThreadPool&, unsigned int = DEF_SPEED);
	void Reverse(nThread::CThreadPool&, unsigned int = DEF_SPEED);
	void TurnLeft(nThread::CThreadPool&, unsigned int = DEF_SPEED);
	void TurnRight(nThread::CThreadPool&, unsigned int = DEF_SPEED);
	void SpinLeft(nThread::CThreadPool&, unsigned int = DEF_SPEED);
	void SpinRight(nThread::CThreadPool&, unsigned int = DEF_SPEED);
	void Brake(nThread::CThreadPool&);

};
#endif // !DRIVE_H
