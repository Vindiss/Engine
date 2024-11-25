#pragma once
#include <memory>
#include <map>
#include <list>
#include <cassert>
#include "Reseau/ServeurImpl.h"

namespace Reseau
{
    class ServeurImpl;

    /**
     * @brief Represents a network server.
     */
    class Serveur
    {
    public:
        /**
         * @brief Default constructor.
         */
        Serveur();

        /**
         * @brief Destructor.
         */
        ~Serveur();

        /**
         * @brief Move constructor.
         * @param other The server to move.
         */
        Serveur(Serveur&& other);

        /**
         * @brief Move assignment operator.
         * @param other The server to move.
         * @return A reference to the moved server.
         */
        Serveur& operator=(Serveur&& other);

        /**
         * @brief Deleted copy constructor.
         */
        Serveur(const Serveur&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         */
        Serveur& operator=(const Serveur&) = delete;

        /**
         * @brief Start the server on the specified port.
         * @param _port The port number to listen on.
         * @return True if the server starts successfully, false otherwise.
         */
        bool Start(unsigned short _port);

        /**
         * @brief Update the server, processing incoming messages.
         */
        void Uptade(); // Typo: should be "Update"

        /**
         * @brief Stop the server and clean up resources.
         */
        void Stop();

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
         * @brief Poll for the next incoming message.
         * @return A unique pointer to the next incoming message, or nullptr if there are no pending messages.
         */
        std::unique_ptr<Reseau::Client::Message> poll();

        /**
         * @brief Get the chat message received.
         * @return The chat message received.
         */
        char GetMessageChat();

    private:
        std::unique_ptr<Reseau::ServeurImpl> mImpl; /**< Pointer to the server implementation. */
    };
}
