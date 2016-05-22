#pragma once
#pragma warning(disable: 4996) // not sure this is a good idea...

//#include <iostream>
#include <winsock2.h>
#include "Aeroplane.h"
#pragma comment(lib,"ws2_32.lib")

class ConnectToNetwork
{
public:

	struct coordinates {
		float positionX;
		float positionY;
		float positionZ;
		float rotX;
		float rotY;
		float rotZ;
	};



	ConnectToNetwork();
	~ConnectToNetwork();

	void init();
	void sendMessage(float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);
	void receive(Aeroplane *aeroplane);

	bool getIsConnected();
	bool getIsThereSomeoneElse();
	bool isConnected;

	

	

private:
	WSADATA WsaDat;
	SOCKET Socket;

	
	bool isThereSomeoneElse;

};

