#include <sstream>
#include <string>

#ifndef HAVE_IPV4_H
#define HAVE_IPV4_H

struct IPAddress {
private:
	char data[4];

public:
	inline IPAddress() {
		for (size_t n = 0; n < 4; n++)
			data[n] = 0;
	}

	//any
	inline static IPAddress Any() {
		return IPAddress();
	}
	
	// localhost
	inline static IPAddress LocalHost() {
		IPAddress ip;
		ip.data[0] = 127;
		ip.data[3] = 1;
		return ip;
	}

	inline IPAddress(int address)
	{
		data[0] = (address >> 24) & 0xFF;
		data[1] = (address >> 16) & 0xFF;
		data[2] = (address >> 8) & 0xFF;
		data[3] = address & 0xFF;
	}

	inline static IPAddress Parse(const std::string& address)
	{
		IPAddress result;
		std::stringstream ss(address);
		std::string item;
		int n = 0;

		while (std::getline(ss, item, '.') && n < 4)
			result.data[n++] = std::stoi(item);

		return result;
	}

	inline std::string ToString() const {
		std::stringstream ss;
		ss << (int)data[0] << "." << (int)data[1] << "." << (int)data[2] << "." << (int)data[3];
		return ss.str();
	}

	inline int AsInt() const
	{
		return (data[0] << 24)
			| (data[1] << 16)
			| (data[2] << 8)
			| data[3];
	}
	
	inline bool operator==(const IPAddress& other) const
	{
		return AsInt() == other.AsInt();
	}
	
	inline bool operator!=(const IPAddress& other) const
	{
		return !(*this == other);
	}
	
	inline bool operator<(const IPAddress& other) const
	{
		return AsInt() < other.AsInt();
	}
	
	inline bool operator>(const IPAddress& other) const
	{
		return AsInt() > other.AsInt();
	}
	
	inline IPAddress(const IPAddress& other)
	{
		for (size_t n = 0; n < 4; n++)
			data[n] = other.data[n];
	}
	
	inline IPAddress(const char* address)
	{
		*this = Parse(address);
	}
};

struct IPEndPoint
{
private:
	IPAddress address;
	unsigned short port;

public:
	IPEndPoint() : address(), port(0) {}
	
	IPEndPoint(const IPEndPoint& other) :
		address(other.address),
		port(other.port)
	{}

	IPEndPoint(const IPAddress& address, unsigned short port) :
		address(address),
		port(port)
	{}

	IPEndPoint(const std::string& address, unsigned short port) :
		address(IPAddress::Parse(address)),
		port(port)
	{}

	std::string ToString() const
	{
		std::ostringstream ss;
		ss << address.ToString() << ":" << port;
		return ss.str();
	}

	bool operator==(const struct IPEndPoint& other) const
	{
		return address == other.address && port == other.port;
	}

	bool operator!=(const struct IPEndPoint& other) const
	{
		return !(*this == other);
	}

	const struct IPAddress& GetAddress() const { return address; }
	unsigned short GetPort() const { return port; }
};

#endif 