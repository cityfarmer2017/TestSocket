#ifdef _WIN64
#include <windows.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include <arpa/inet.h>
#endif

#include <iostream>

constexpr auto SOCK_CNT = 2000;

int main(int argc, char* argv[])
{
#ifdef _WIN64
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif

	auto port = uint16_t{8080};
	if (argc >= 2)
	{
		port = atoi(argv[1]);
	}

	auto sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		std::cout << "create socket failed!" << std::endl;
		return static_cast<int>(sock);
	}

	auto saddr = sockaddr_in{};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(0);

	if (bind(sock, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)))
	{
		std::cout << "bind port " << port << " failed!" << std::endl;
		return -1;
	}

	std::cout << "bind port " << port << " succeeded!" << std::endl;

	char c;
	std::cin >> c;

	return 0;
}