#pragma once

#include <cassert>
#include "Reseau/Client/ConnectionHandler.h"
#include "Reseau/Client/SendingHandler.h"
#include "Reseau/Client/ReceptionHandler.h"
#include "Reseau/Sockets/Socket.h"

namespace Reseau
{
    namespace Client
    {
        /**
         * @brief Class implementing the internal workings of a network client.
         */
        class ClientImpl
        {
            enum class State {
                Connecting, /**< State representing connection in progress. */
                Connected,  /**< State representing established connection. */
                Disconnected /**< State representing disconnection. */
            };

        public:
            /**
             * @brief Default constructor.
             */
            ClientImpl() = default;

            /**
             * @brief Destructor.
             */
            ~ClientImpl();

            /**
             * @brief Initializes the client with the given socket and address.
             *
             * @param sckt The socket to use.
             * @param addr The address to associate with the socket.
             * @return true if initialization succeeds, false otherwise.
             */
            bool Init(SOCKET&& sckt, const sockaddr_in& addr);

            /**
             * @brief Establishes a connection with the server at the specified address and port.
             *
             * @param ipaddress The server's IP address.
             * @param port The server's port.
             * @return true if the connection succeeds, false otherwise.
             */
            bool Connect(const std::string& ipaddress, unsigned short port);

            /**
             * @brief Disconnects the client from the server.
             */
            void Disconnect();

            /**
             * @brief Sends data to the server.
             *
             * @param data The data to send.
             * @param len The length of the data.
             * @return true if sending succeeds, false otherwise.
             */
            bool Send(const unsigned char* data, unsigned int len);

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
            char GetMessageChat() { return msgChat; }

            /**
             * @brief Returns the client's identifier.
             *
             * @return The client's identifier.
             */
            uint64_t Id() const { return static_cast<uint64_t>(mSocket); }

            /**
             * @brief Returns the client's destination address.
             *
             * @return The client's destination address.
             */
            const sockaddr_in& DestinationAddress() const { return mAddress; }

        private:
            /**
             * @brief Processing performed when a connection is established.
             *
             * @param addr The address to which the connection is established.
             */
            void OnConnected(const sockaddr_in& addr);

        private:
            Reseau::Client::ConnectionHandler mConnectionHandler; /**< Connection handler. */
            Reseau::Client::SendingHandler mSendingHandler;       /**< Sending handler. */
            Reseau::Client::ReceptionHandler mReceivingHandler;   /**< Reception handler. */
            SOCKET mSocket{ INVALID_SOCKET };                     /**< Client's socket. */
            State mState{ State::Disconnected };                  /**< Current state of the client. */
            sockaddr_in mAddress{ 0 };                            /**< Destination address of the client. */
            char msgChat;                                         /**< Chat message. */
        };
    }
}
