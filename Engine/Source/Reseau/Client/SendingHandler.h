#pragma once

#include <list>
#include <vector>
#include <cassert>
#include <numeric>
#include "Reseau/Sockets/Socket.h"

namespace Reseau
{
    namespace Client
    {
        /**
         * @brief Handles sending data over a socket in a packetized manner.
         */
        class SendingHandler
        {
            enum class State {
                Idle,   /**< Idle state. */
                Header, /**< Header state. */
                Data    /**< Data state. */
            };
        public:
            /**
             * @brief Default constructor.
             */
            SendingHandler() = default;

            /**
             * @brief Initialize the sending handler with a socket.
             * @param sckt The socket to use for sending data.
             */
            void init(SOCKET sckt);

            /**
             * @brief Send data over the socket.
             * @param data Pointer to the data to send.
             * @param datalen Length of the data to send.
             * @return True if the data is successfully queued for sending, false otherwise.
             */
            bool Send(const unsigned char* data, unsigned int datalen);

            /**
             * @brief Update the sending handler, sending queued data if necessary.
             */
            void Update();

            /**
             * @brief Get the size of the data queue.
             * @return The size of the data queue.
             */
            size_t QueueSize() const;

        private:
            /**
             * @brief Send the pending buffer over the socket.
             * @return True if the buffer is successfully sent, false otherwise.
             */
            bool sendPendingBuffer();

            /**
             * @brief Prepare the next header for the packet.
             */
            void prepareNextHeader();

            /**
             * @brief Prepare the next data for the packet.
             */
            void prepareNextData();

            std::list<std::vector<unsigned char>> mQueueingBuffers; /**< List of queued data buffers. */
            std::vector<unsigned char> mSendingBuffer; /**< Buffer for sending data. */
            SOCKET mSocket{ INVALID_SOCKET }; /**< The socket for sending data. */
            State mState{ State::Idle }; /**< Current state of the sending handler. */
        };
    }
}
