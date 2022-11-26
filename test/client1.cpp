#include "udp_hole_punch.h"
const char* SERVER_IP = "a.b.c.d"; //your server
const int SERVERUDPPORT = 10000;
const int MYPORT = 22222;
int main()
{
	SOCKET udpfd;
	if (!udpPunchSide(udpfd, SERVER_IP, SERVERUDPPORT, MYPORT))
	{
		closesocket(udpfd);
		WSACleanup();
		return 0;
	}
	//do other thing...

	//end
	closesocket(udpfd);
	WSACleanup();
	return 0;
}
