#include "ConnectToNetwork.h"


ConnectToNetwork::ConnectToNetwork()
{
	init();
}


ConnectToNetwork::~ConnectToNetwork()
{
}

void ConnectToNetwork::init()
{
	//// set to true, yet if any stage fails then flip to false
	isConnected = true;

	// initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		// Winsock initialization failed
		WSACleanup();
		isConnected = false;
	}

	// Create our socket

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		// Socket creation Failed

		WSACleanup();
		isConnected = false;
	}

	// Resolve IP address for hostname
	struct hostent *host;
	if ((host = gethostbyname("localhost")) == NULL)
	{
		//Failed to resolve hostname
		WSACleanup();
		isConnected = false;
	}

	// Setup our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(5555);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	// Attempt to connect to server
	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		//Failed to establish connection with server
		WSACleanup();
		isConnected = false;
	}

	// If iMode!=0, non-blocking mode is enabled.
	u_long iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);
}


void ConnectToNetwork::sendMessage(float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ) {
	//int bytesSent;
	//int bytesRecv = SOCKET_ERROR;
	//char sendbuf[200] = "This string is a test data from client";
	//bytesSent = send(Socket, sendbuf, strlen(sendbuf), 0);

	coordinates planePos;
	planePos.positionX = posX;
	planePos.positionY = posY;
	planePos.positionZ = posZ;
	planePos.rotX = rotX;
	planePos.rotX = rotY;
	planePos.rotX = rotZ;

	//unsigned NetInt;
	//NetInt = htonl((unsigned)posX);

	send(Socket, (char *)&planePos, sizeof(coordinates), 0);
}

void ConnectToNetwork::receive(Aeroplane *aeroplane) {

	// untill we receive a message, assume there is noone else
	isThereSomeoneElse = false;

	// create a new struct to hold the receiving message (plane coordinates for now)
	coordinates otherPlanePos;

	// there is a message incoming from the server
 	int inDataLength = recv(Socket, (char*)&otherPlanePos, sizeof(coordinates), 0);

	// use the incoming struct to update the aeroplane position
	aeroplane->plane->setPositionX(otherPlanePos.positionX);
	aeroplane->plane->setPositionY(otherPlanePos.positionY);
	aeroplane->plane->setPositionZ(otherPlanePos.positionZ);

	aeroplane->plane->setRotationX(otherPlanePos.rotX);
	aeroplane->plane->setRotationY(otherPlanePos.rotY);
	aeroplane->plane->setRotationZ(otherPlanePos.rotZ);


	// there must be someone else if we received a message
	if (inDataLength != -1) isThereSomeoneElse = true;
	
}

bool ConnectToNetwork::getIsConnected()
{
	return this->isConnected;
}

bool ConnectToNetwork::getIsThereSomeoneElse()
{
	return isThereSomeoneElse;
}

