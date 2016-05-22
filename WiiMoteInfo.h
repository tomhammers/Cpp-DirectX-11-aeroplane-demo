#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "DXUT.h"

//using namespace std;

class WiiMoteInfo {
private:
	std::ofstream outputFile;
	std::wifstream fileStream;
	std::wstring line;

public:
	WiiMoteInfo();
	~WiiMoteInfo();

	// public variables!!!!!!!!!!!!
	bool buttonAPressed;
	float roll = 0.0;
	float pitch = 0.0;
	float yaw = 0.0;

	void setWiiMoteData();
	bool getButtonAPressed();

};