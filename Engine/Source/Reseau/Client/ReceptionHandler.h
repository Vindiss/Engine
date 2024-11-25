#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include "Reseau/Sockets/Socket.h"
#include "Reseau/Client/Message.h"
#include "Reseau/Sockets/Error.h"

namespace Reseau
{
    namespace Client
    {
        /**
         * @brief Handles receiving data over a socket.
         */
        class ReceptionHandler
        {
            enum class State {
                Header, /**< Header state. */
                Data    /**< Data state. */
            };
        public:
            /**
             * @brief Default constructor.
             */
            ReceptionHandler() = default;

            /**
             * @brief Initialize the reception handler with a socket.
             * @param sckt The socket to use for receiving data.
             */
            void init(SOCKET sckt);

            /**
             * @brief Receive a message over the socket.
             * @return A unique pointer to the received message.
             */
            std::unique_ptr <Reseau::Client::Message> recv();

            /**
             * @brief Get the received data.
             * @return The received data.
             */
            char GetData() { return data; }

        private:
            /**
             * @brief Get a pointer to the start of missing data in the buffer.
             * @return A pointer to the start of missing data.
             */
            inline char* missingDataStartBuffer() { return reinterpret_cast<char*>(mBuffer.data() + mReceived); }

            /**
             * @brief Get the length of missing data in the buffer.
             * @return The length of missing data.
             */
            inline int missingDataLength() const { return static_cast<int>(mBuffer.size() - mReceived); }

            /**
             * @brief Start receiving header data.
             */
            void startHeaderReception();

            /**
             * @brief Start receiving data.
             */
            void startDataReception();

            /**
             * @brief Start receiving data with expected length and set new state.
             * @param expectedDataLength The expected length of the data to receive.
             * @param newState The new state after starting reception.
             */
            void startReception(unsigned int expectedDataLength, State newState);

            std::vector<unsigned char> mBuffer; /**< Buffer for received data. */
            unsigned int mReceived; /**< Amount of received data. */
            SOCKET mSckt{ INVALID_SOCKET }; /**< The socket for receiving data. */
            State mState; /**< Current state of the reception handler. */
            char data; /**< Received data. */
        };
    }
}