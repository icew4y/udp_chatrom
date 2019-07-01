// client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <assert.h>
#include <thread>
#include <chrono>

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


char* performance_message(const char* p, size_t len, uint32_t* plen) {
	assert(len > 0);
	assert(p != nullptr);

	char* newbuf = new char[len + sizeof(uint32_t)];
	uint32_t newlen = len + sizeof(uint32_t);
	memcpy(newbuf, &newlen, sizeof(uint32_t));
	memcpy(newbuf + sizeof(uint32_t), p, len);
	*plen = newlen;
	return newbuf;
}

int main()
{
	WSADATA wsa;
	int r = WSAStartup(MAKEWORD(2, 2), &wsa);


	SOCKET fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));

	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);

	int msglen = sizeof(message) + sizeof(Login);
	message *msg = (message *)new char[msglen];
	memset(msg, 0, msglen);
	((message*)msg)->receiver = SERVER;
	((message*)msg)->cmd = LOGIN;

	Login* login = (Login*)((char *)msg + sizeof(message));
	memcpy(login->username, "icew4y", strlen("icew4y"));
	memcpy(login->password, "123456", strlen("123456"));
	

	uint32_t newlen = 0;
	char* pmsg = performance_message((const char *)msg, msglen, &newlen);
	delete msg;


	while (true)
	{
		sendto(fd, pmsg, newlen, 0, (sockaddr*)& sin, sizeof(sin));
		break;
		this_thread::sleep_for(chrono::seconds(4));
	}
	
	delete pmsg;
}

