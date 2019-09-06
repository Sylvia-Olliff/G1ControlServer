#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H
#include <mutex>
namespace Commands {
	static void DriveCommand(std::mutex&, bool, bool, int, bool, bool, int);
	static void ServoPulse(std::mutex&, int, int);
	static void ServoScan(std::mutex&, int);
	static void SetPin(std::mutex&, int, bool);
	static void SetColor(std::mutex&, bool, bool, bool);
	static float Distance();
	static void GetDistance(std::mutex&, float&);
	static void bubble(unsigned long* a, int n);
}
#endif // !COMMANDS_H
