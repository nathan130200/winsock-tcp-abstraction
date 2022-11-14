#include <WinSock2.h>
#include <string>
#include <tuple>

#ifndef HAVE_SOCKET_H
#define HAVE_SOCKET_H
#pragma comment(lib, "ws2_32.lib")
#include "ipv4.h"

class TCPClient;

typedef std::tuple<bool, TCPClient> TCPServerAcceptResult;

class TCPServer
{

private:

	SOCKET m_socket;
	IPEndPoint m_endpoint;

	void Init();

public:

	TCPServer(IPEndPoint ep) :
		m_endpoint(ep),
		m_socket(INVALID_SOCKET)
	{
		Init();
	}

	~TCPServer() = default;

	void Listen(size_t backlog = SOMAXCONN);
	TCPServerAcceptResult Accept();
};

class TCPClient
{

private:

	SOCKET m_socket;
	IPEndPoint m_localEndpoint;

public:
	TCPClient();
	TCPClient(SOCKET fd, IPEndPoint localEndpoint);
	~TCPClient();

	size_t Recv(char* buffer, size_t len);
	size_t Send(char* buffer, size_t len);
};

#endif 