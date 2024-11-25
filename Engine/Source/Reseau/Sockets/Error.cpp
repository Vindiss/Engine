#include "Reseau/Sockets/Error.h"

int Reseau::Sockets::Error::GetError()
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}
