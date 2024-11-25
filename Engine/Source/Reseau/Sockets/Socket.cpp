#include "Socket.h"


bool Reseau::Sockets::Socket::Start()
		{
#ifdef _WIN32
			WSAData wsaData;
			return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
			return true;
#endif
		}
		void Reseau::Sockets::Socket::Release()
		{
#ifdef _WIN32
			WSACleanup();
#endif
		}
		int Reseau::Sockets::Socket::GetError()
		{
#ifdef _WIN32
			int error = WSAGetLastError();
			if (error != 0)
			{
				char errorMessage[256];
				FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					errorMessage, sizeof(errorMessage), NULL);

				OutputDebugStringA(errorMessage);
			}
			return error;
#else
			return errno;
#endif
		}
		void Reseau::Sockets::Socket::CloseSocket(SOCKET s)
		{
#ifdef _WIN32
			closesocket(s);
#else
			close(s);
#endif
		}

std::string Reseau::Sockets::Socket::GetAddress(const sockaddr_in& addr)
{
	char buff[INET6_ADDRSTRLEN] = { 0 };
	return inet_ntop(addr.sin_family, (void*)&(addr.sin_addr), buff, INET6_ADDRSTRLEN);
}

bool Reseau::Sockets::Socket::SetNonBlocking(SOCKET socket)
{
	u_long mode = 1;
	return ioctlsocket(socket, FIONBIO, &mode);
}

bool Reseau::Sockets::Socket::SetReuseAddr(SOCKET socket)
{
	int optval = 1;
	return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval));
}
