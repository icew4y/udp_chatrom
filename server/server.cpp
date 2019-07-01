// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#include "message_buf.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;



#define SERVER 0
#define ALL	1


#define LOGIN 1000
#define REGISTER 1001





struct message
{
	uint8_t receiver;
	uint16_t cmd;
	char data[0];
};

struct Login
{
	char username[16];
	char password[32];
};


#define BUF_SIZE 1024

void ServerReceiver(const SOCKET fd) {
	sockaddr_in clientAddr;
	int			addrLen = sizeof(clientAddr);

	message_buf recvbuf;
	int hsize = 0;

	while (true) {

		//UDP 直接从客户端接收数据
		char buf[BUF_SIZE] = { 0 };
		int irec = recvfrom(fd, buf, BUF_SIZE, 0, (sockaddr*)&clientAddr, &addrLen);
		if (irec == SOCKET_ERROR) {
			cout << "recvform failed wich code:" << WSAGetLastError() << endl;
			break;
		}

		if (irec == 0)
			continue;

		recvbuf.add_msg(buf, irec);

		if (hsize == 0) {
			hsize = recvbuf.get_header(4);
			if (hsize == 0)
				continue;
		}
		int datalen = hsize - 4;
		char* readbuf = new char[datalen];
		int len = recvbuf.get_msg(readbuf, datalen);
		if (len == 0) {
			continue;
		}


		message* p_msg = (message*)readbuf;
		switch (p_msg->receiver)
		{
		case SERVER: {
			switch (p_msg->cmd)
			{
			case REGISTER:
				break;
			}
			case LOGIN: {
				cout << "LOGIN:" << endl;
				Login* login = (Login*)p_msg->data;
				cout << "username:" << login->username << endl;
				cout << "password:" << login->password << endl;
				break;
			}
			break;
		}
		case ALL: {
			break;
		}
		default:
			cout << "unknow recevier:" << p_msg->receiver << endl;
			break;
		}





		hsize = 0;
	}
}

int main()
{
	WSADATA wsa;
	int r = WSAStartup(MAKEWORD(2,2), &wsa);


	SOCKET fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));

	inet_pton(AF_INET, "0.0.0.0", &sin.sin_addr);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);

	r = ::bind(fd, (sockaddr*)&sin, sizeof(sin));
	if (SOCKET_ERROR == r) {
		cout << "bind failed with code:" << WSAGetLastError() << endl;
		return 0;
	}

	ServerReceiver(fd);
	closesocket(fd);
	WSACleanup();
}

