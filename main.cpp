#ifdef _WIN64
//#include <windows.h>
 #include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <array>
#include <algorithm>

constexpr auto MAX_SOCK_CNT = 10;

int main(int argc, char* argv[])
{
#ifdef _WIN64
    WSADATA ws;
    if (auto rc = WSAStartup(MAKEWORD(2, 2), &ws))
    {
        return rc;
    }
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
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)))
    {
        std::cout << "bind port " << port << " failed!" << std::endl;
        return -1;
    }

    std::cout << "bind port " << port << " succeeded!" << std::endl;

    if (listen(sock, MAX_SOCK_CNT) == -1) {
        std::cout << "listen() failed!" << std::endl;
        return -1;
    }

    auto caddr = sockaddr_in{};
    auto len = socklen_t{sizeof(caddr)};

    auto client = accept(sock, reinterpret_cast<sockaddr*>(&caddr), &len);
    if (client == -1)
    {
        std::cout << "accept() failed!" << std::endl;
        return -1;
    }

    std::cout << "accept client: " << client << "\n";

    auto buff = std::array<char, 1024>{};
    auto cip = inet_ntop(AF_INET, reinterpret_cast<const void*>(&(caddr.sin_addr)), buff.data(), 16);
    auto cport = ntohs(caddr.sin_port);

    std::cout << "\tclient ip is: " << cip << "\n\tclient port is: " << cport << "\n";

    while (1)
    {
        buff.fill(0);

        auto rlen = recv(client, buff.data(), sizeof(buff)-1, 0);
        if (-1 == rlen)
        {
            std::cout << "recv() failed!" << std::endl;
            return -1;
        }

        auto buff_str = std::string(buff.data());
        buff_str.erase((std::find_if_not(buff_str.rbegin(), buff_str.rend(), isspace)).base(), buff_str.end());
        if (buff_str == "quit") {
            break;
        }

        std::cout << "received: " << buff.data();
    }

#ifdef _WIN64
    closesocket(client);
#else
    close(client);
#endif

    // char c;
    // std::cin >> c;

    return 0;
}