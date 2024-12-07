#ifdef _WIN64
#include <windows.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <iostream>

constexpr auto SOCK_CNT = 2000;

int main(int argc, char* argv[])
{
#ifdef _WIN64
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif

	for (auto i = 0; i < SOCK_CNT; ++i)
	{
		auto sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0)
		{
			std::cout << "create socket failed!" << std::endl;
			return sock;
		}

		std::cout << "sock: " << sock << "\n";
#ifdef _WIN64
		closesocket(sock);
#else
		close(sock);
#endif
	}

	char c;
	std::cin >> c;

	return 0;
}