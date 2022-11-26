#include "udp_hole_punch.h"
const char* SERVER_IP = "a.b.c.d"; //your server
const int SERVERUDPPORT = 10000;
const int MYPORT = 22222;
int main()
{
	SOCKET udpfd;
	pair<string, int> gateway = udpPunchedSide(udpfd, SERVER_IP, SERVERUDPPORT, MYPORT);
	if (gateway.first == "")
	{
		closesocket(udpfd);
		WSACleanup();
		return 0;
	}
	const char* gateway_ip = gateway.first.c_str();
	const int gateway_port = gateway.second;

	//do other thing...

	//end
	closesocket(udpfd);
	WSACleanup();
	return 0;
}
