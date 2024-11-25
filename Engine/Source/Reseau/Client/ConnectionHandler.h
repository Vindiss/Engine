#pragma once

#include <vector>
#include <cassert>
#include <memory>
#include "Reseau/Sockets/Socket.h"
#include "Reseau/Client/Message.h"
#include "Reseau/Sockets/Error.h"

namespace Reseau
{
    namespace Client
    {
        /**
         * @brief Class responsible for handling connection-related operations.
         */
        class ConnectionHandler
        {
        public:
            /**
             * @brief Default constructor.
             */
            ConnectionHandler() = default;

            /**
             * @brief Establishes a connection with the specified address and port.
             *
             * @param sckt The socket to use for the connection.
             * @param address The IP address of the server.
             * @param port The port of the server.
             * @return true if the connection succeeds, false otherwise.
             */
            bool connect(SOCKET sckt, const std::string& address, unsigned short port);

            /**
             * @brief Polls for any incoming connection.
             *
             * @return A unique pointer to the connection if available, nullptr otherwise.
             */
			std::unique_ptr<Reseau::Client::Connection> poll();
            /**
            * @brief Retrieves the address of the connected server.
            *
            * @return The sockaddr_in structure representing the connected server's address.
            */
            const sockaddr_in& getConnectedAddress() const { return mConnectedAddress; }

        private:
            pollfd mFd{ 0 }; /**< File descriptor for polling. */
            sockaddr_in mConnectedAddress; /**< Address of the connected server. */
            std::string mAddress; /**< IP address of the server to connect to. */
            unsigned short mPort; /**< Port of the server to connect to. */
        };
    }
}