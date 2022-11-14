#include "tcpsocket.h"
#include <WS2tcpip.h>
#include "strfmt.h"

void TCPServer::Init()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		auto str = StrFmt("Cannot create socket instance. Code: %d", WSAGetLastError());
		throw std::exception(str.Get());
	}

	struct sockaddr_in hints;

	auto host = m_endpoint.GetAddress().ToString();

	if (host == "*" || host == "0.0.0.0")
		hints.sin_addr.s_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, host.c_str(), &hints.sin_addr);


	hints.sin_port = htons(m_endpoint.GetPort());
	hints.sin_family = AF_INET;

	bind(m_socket, (const sockaddr*)&hints, sizeof(hints));
}

void TCPServer::Listen(size_t backlog)
{
	if (!listen(m_socket, backlog))
		return;

	auto str = StrFmt("Socket listen failed. Code: %d", WSAGetLastError());
	throw std::exception(str.Get());
}

[[nodiscard]]
TCPServerAcceptResult TCPServer::Accept()
{
	sockaddr addr;
	SOCKET client = accept(m_socket, &addr, NULL);

	if (client == INVALID_SOCKET)
		return TCPServerAcceptResult(false, {});

	auto addrIn = (sockaddr_in*)&addr;
	unsigned short port = ntohs(addrIn->sin_port);

	auto localEP = IPEndPoint(IPAddress(addrIn->sin_addr.s_addr), port);
	return TCPServerAcceptResult(true, TCPClient(client, localEP));
}

TCPClient::TCPClient() :
	m_socket(INVALID_SOCKET)
{
	printf("TCPClient::ctor(): INVALID_SOCKET");
}

TCPClient::TCPClient(SOCKET socket, IPEndPoint localEP) :
	m_socket(socket),
	m_localEndpoint(localEP)
{
	printf("::TCPClient(): fd: %d; ep: %s\n", m_socket, m_localEndpoint.ToString().c_str());
}

TCPClient::~TCPClient()
{
	if (m_socket != INVALID_SOCKET)
	{
		printf("::~TCPClient(): fd: %d\n", m_socket);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

size_t TCPClient::Recv(char* buffer, size_t len)
{
	return recv(m_socket, buffer, len, 0);
}

size_t TCPClient::Send(char* buffer, size_t len)
{
	return send(m_socket, buffer, len, 0);
}