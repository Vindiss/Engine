#ifdef _WIN32
#if _MSC_VER >= 1800
#include <WS2tcpip.h>
#else
#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))
typedef int socklen_t;
#endif
typedef unsigned long nfds_t;
#define poll WSAPoll
#include <WinSock2.h>
#ifdef _MSC_VER
#if _WIN32_WINNT >= _WIN32_WINNT_WINBLUE
#pragma comment(lib, "Ws2_32.lib")
#else
#pragma comment(lib, "wsock32.lib")
#endif
#endif
#else
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
#include <arpa/inet.h> // hton*, ntoh*, inet_addr
#include <unistd.h>  // close
#include <cerrno> // errno
#define SOCKET int
#define INVALID_SOCKET ((int)-1)
#endif 

#include <string>

namespace Reseau
{
	namespace Sockets
	{
		namespace Socket
		{
			/**
			 * @brief Start the networking subsystem.
			 * @return True if successful, false otherwise.
			 */
			bool Start();

			/**
			 * @brief Release resources used by the networking subsystem.
			 */
			void Release();

			/**
			 * @brief Get the last error code.
			 * @return The last error code.
			 */
			int GetError();

			/**
			 * @brief Close a socket.
			 * @param socket The socket to close.
			 */
			void CloseSocket(SOCKET socket);

			/**
			 * @brief Get the string representation of an IP address.
			 * @param addr The sockaddr_in structure containing the IP address.
			 * @return The string representation of the IP address.
			 */
			std::string GetAddress(const sockaddr_in& addr);

			/**
			 * @brief Set a socket to non-blocking mode.
			 * @param socket The socket to set.
			 * @return True if successful, false otherwise.
			 */
			bool SetNonBlocking(SOCKET socket);

			/**
			 * @brief Set a socket to reuse address mode.
			 * @param socket The socket to set.
			 * @return True if successful, false otherwise.
			 */
			bool SetReuseAddr(SOCKET socket);
		}
	}
}
