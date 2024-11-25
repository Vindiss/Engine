#pragma once

#include <string>
#include <memory>
#include "Reseau/Client/ClientImpl.h"
#include "Reseau/UDP/DistantClient.h"

namespace Reseau
{
    namespace Client
    {
        /**
         * @brief Alias for the header type used in communication.
         */
        using HeaderType = uint16_t;

        /**
         * @brief Size of the header in communication data.
         */
        static const unsigned int HeaderSize = sizeof(HeaderType);

        // Forward declaration
        class ClientImpl;

        /**
         * @brief Class representing a network client.
         */
        class Client
        {
        public:
            friend class DistantClient;

            /**
             * @brief Default constructor.
             */
            Client();

            /**
             * @brief Deleted copy constructor.
             */
            Client(const Client&) = delete;

            /**
             * @brief Deleted copy assignment operator.
             */
            Client& operator=(const Client&) = delete;

            /**
             * @brief Move constructor.
             */
            Client(Client&&);

            /**
             * @brief Move assignment operator.
             */
            Client& operator=(Client&&);

            /**
             * @brief Destructor.
             */
            ~Client();

            /**
             * @brief Initializes the client with the given socket and address.
             *
             * @param sckt The socket to use.
             * @param addr The address to associate with the socket.
             * @return true if initialization succeeds, false otherwise.
             */
            bool init(SOCKET&& sckt, const sockaddr_in& addr);

            /**
             * @brief Establishes a connection with the server at the specified address and port.
             *
             * @param ipaddress The server's IP address.
             * @param port The server's port.
             * @return true if the connection succeeds, false otherwise.
             */
            bool connect(const std::string& ipaddress, unsigned short port);

            /**
             * @brief Disconnects the client from the server.
             */
            void disconnect();

            /**
             * @brief Sends data to the server.
             *
             * @param data The data to send.
             * @param len The length of the data.
             * @return true if sending succeeds, false otherwise.
             */
            bool send(const unsigned char* data, unsigned int len);

            /**
             * @brief Extracts the first message waiting in the client's queue.
             *
             * @return A unique pointer to the extracted message.
             */
            std::unique_ptr<Message> poll();

            /**
             * @brief Retrieves a chat message.
             *
             * @return The character representing the chat message.
             */
            char GetMessageChat();

            /**
             * @brief Returns the client's identifier.
             *
             * @return The client's identifier.
             */
            uint64_t id() const;

            /**
             * @brief Returns the client's destination address.
             *
             * @return The client's destination address.
             */
            const sockaddr_in& destinationAddress() const;

        private:
            std::unique_ptr<ClientImpl> mImpl; /**< Client implementation. */
        };
    }
}
