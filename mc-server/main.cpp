#include "tcpsocket.h"
#include "strfmt.h"
#include <exception>
#include "main.h"
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char** argv)
{
	try {
		WSAData wsaData;
		ZeroMemory(&wsaData, sizeof(WSAData));
		int code = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (code) {
			auto msg = StrFmt("WSA Startup failed. Code: %d", code);
			throw new std::exception(msg.Get());
		}

		auto ep = IPEndPoint(IPAddress::Any(), 25565);
		TCPServer server(ep);
		server.Listen();

		cout << "listening on: " << ep.ToString() << "!\n";

		while (true) {
			auto result = server.Accept();

			if (std::get<0>(result)) {
				auto client = std::get<1>(result);
			}
			else {
				cout << "\rnot pending connection";
			}

			this_thread::sleep_for(1ms);
		}
	}
	catch (std::exception& e) {
		printf("ERROR: %s\n", e.what());
	}

	return 0;
}