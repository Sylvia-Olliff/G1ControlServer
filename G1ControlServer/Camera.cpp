#include <wiringPi.h>
#include <iostream>
#include <mutex>
#include "Camera.h"
#include "Pins.h"
#include "Commands.cpp"
#include "FDVHVDU/ThreadPool/header/CThreadPool.hpp"

Camera::Camera(nThread::CThreadPool& TPool)
{
	pinMode(CAMERA_SERVO_H, OUTPUT);
	pinMode(CAMERA_SERVO_V, OUTPUT);

	TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, 90);
	TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, 90);

	TPool.join_all();
	CameraHPos = 90;
	CameraVPos = 90;
};

Camera::~Camera()
{
	pinMode(CAMERA_SERVO_H, INPUT);
	pinMode(CAMERA_SERVO_V, INPUT);
}

void Camera::CameraLeft(nThread::CThreadPool& TPool, int distance)
{
	int newAngle = CameraHPos + distance;
	if (newAngle > 180)
		newAngle = 180;

	if (distance > 10) {
		int increment = (int)(distance / 10);
		for (int i = 0; i <= distance; i += increment) {
			TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, CameraHPos + increment);
			CameraHPos += increment;
		}
		if (distance % 10 > 0) {
			TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, CameraHPos + (distance % 10));
			CameraHPos += (distance % 10);
		}
	}
	else {
		TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, newAngle);
		CameraHPos = newAngle;
	}
	return;
}

void Camera::CameraRight(nThread::CThreadPool& TPool, int distance)
{
	int newAngle = CameraHPos - distance;
	if (newAngle < 0)
		newAngle = 0;

	if (distance > 10) {
		int increment = (int)(distance / 10);
		for (int i = 0; i <= distance; i += increment) {
			TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, CameraHPos - increment);
			CameraHPos -= increment;
		}
		if (distance % 10 > 0) {
			TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, CameraHPos - (distance % 10));
			CameraHPos -= (distance % 10);
		}
	}
	else {
		TPool.add_and_detach(Commands::ServoPulse, MutH, CAMERA_SERVO_H, newAngle);
		CameraHPos = newAngle;
	}
	return;
}

void Camera::CameraUp(nThread::CThreadPool& TPool, int distance)
{
	int newAngle = CameraVPos + distance;
	if (newAngle > 180)
		newAngle = 180;

	if (distance > 10) {
		int increment = (int)(distance / 10);
		for (int i = 0; i <= distance; i += increment) {
			TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, CameraVPos + increment);
			CameraVPos += increment;
		}
		if (distance % 10 > 0) {
			TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, CameraVPos + (distance % 10));
			CameraVPos += (distance % 10);
		}
	}
	else {
		TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, newAngle);
		CameraVPos = newAngle;
	}
	return;
}

void Camera::CameraDown(nThread::CThreadPool& TPool, int distance)
{
	int newAngle = CameraVPos - distance;
	if (newAngle < 35)
		newAngle = 35;

	if (distance > 10) {
		int increment = (int)(distance / 10);
		for (int i = 0; i <= distance; i += increment) {
			TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, CameraVPos - increment);
			CameraVPos -= increment;
		}
		if (distance % 10 > 0) {
			TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, CameraVPos - (distance % 10));
			CameraVPos -= (distance % 10);
		}
	}
	else {
		TPool.add_and_detach(Commands::ServoPulse, MutV, CAMERA_SERVO_V, newAngle);
		CameraVPos = newAngle;
	}
	return;
}
