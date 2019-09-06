#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <mutex>
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"
class Camera {
private:
	std::mutex MutH;
	std::mutex MutV;
public:
	int CameraHPos;
	int CameraVPos;
	Camera(nThread::CThreadPool&);
	~Camera();
	void CameraLeft(nThread::CThreadPool&, int);
	void CameraRight(nThread::CThreadPool&, int);
	void CameraUp(nThread::CThreadPool&, int);
	void CameraDown(nThread::CThreadPool&, int);
};
#endif // !CAMERA_H
