#pragma once

#include <array>

namespace Reseau
{
    namespace UDP
    {
        /**
         * @brief Struct representing a datagram for UDP communication.
         */
        struct Datagram
        {
            using ID = uint16_t;

            /**
             * @brief Struct representing the header of the datagram.
             */
            struct Header
            {
                ID id; /**< Datagram ID. */
                ID ack; /**< Acknowledgement ID. */
                uint64_t previousAcks; /**< Bit mask of previous acknowledgements. */
            };

            static constexpr size_t BufferMaxSize = 1400; /**< Maximum buffer size for the datagram. */
            static constexpr size_t DataMaxSize = BufferMaxSize - sizeof(ID); /**< Maximum data size for the datagram. */

            Header header; /**< Header of the datagram. */
            std::array<uint8_t, DataMaxSize> data; /**< Data payload of the datagram. */
            size_t datasize{ 0 }; /**< Size of the data payload. */
            static const unsigned int HeaderSize = sizeof(Header); /**< Size of the datagram header. */
        };
    }
}
