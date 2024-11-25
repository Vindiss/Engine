#pragma once

#include <Winsock2.h>
#include <memory>
#include <cassert>
#include "Reseau/Client/Message.h"
#include "Reseau/Sockets/Socket.h"
#include "Reseau/Sockets/Error.h"
#include "Reseau/Client/Client.h"

#include <map>

namespace Reseau
{
    /**
     * @brief Implementation of a server for network communication.
     */
    class ServeurImpl
    {
    public:
        /**
         * @brief Constructor.
         */
        ServeurImpl() = default;

        /**
         * @brief Destructor.
         */
        ~ServeurImpl();

        /**
         * @brief Start the server on the specified port.
         * @param _port The port number to listen on.
         * @return True if the server starts successfully, false otherwise.
         */
        bool Start(unsigned short _port);

        /**
         * @brief Stop the server and clean up resources.
         */
        void Stop();

        /**
         * @brief Update the server, processing incoming messages.
         */
        void Update();

        /**
         * @brief Poll for the next incoming message.
         * @return A unique pointer to the next incoming message, or nullptr if there are no pending messages.
         */
        std::unique_ptr<Reseau::Client::Message> poll();

        /**
         * @brief Send a message to a specific client.
         * @param clientid The ID of the client to send the message to.
         * @param data Pointer to the message data.
         * @param len Length of the message data.
         * @return True if the message is sent successfully, false otherwise.
         */
        bool SendTo(uint64_t clientid, const unsigned char* data, unsigned int len);

        /**
         * @brief Send a message to all connected clients.
         * @param data Pointer to the message data.
         * @param len Length of the message data.
         * @return True if the message is sent successfully to all clients, false otherwise.
         */
        bool SendToAll(const unsigned char* data, unsigned int len);

        /**
         * @brief Get the chat message received.
         * @return The chat message received.
         */
        char GetMessageChat() { return msgChat; }

    private:
        std::map<uint64_t, Reseau::Client::Client> mClients; /**< Map of connected clients. */
        std::list<std::unique_ptr<Reseau::Client::Message>> mMessages; /**< List of incoming messages. */
        SOCKET mSocket{ INVALID_SOCKET }; /**< The server socket. */
        char msgChat; /**< The chat message. */
    };
}
