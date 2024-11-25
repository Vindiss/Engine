#pragma once

#include <cstdint>
#include <limits>
#include <cassert>

namespace Reseau
{
    namespace UDP
    {
        namespace Utils
        {
            /**
             * @brief Check if a sequence number is newer than the last one received.
             * @param sNew The new sequence number.
             * @param sLast The last sequence number received.
             * @return True if the new sequence number is newer, false otherwise.
             */
            bool IsSequenceNewer(uint16_t sNew, uint16_t sLast);

            /**
             * @brief Calculate the difference between two sequence numbers.
             * @param sNew The newer sequence number.
             * @param sLast The older sequence number.
             * @return The difference between the two sequence numbers.
             */
            uint16_t SequenceDiff(uint16_t sNew, uint16_t sLast);

            /**
             * @brief Set a specific bit in a bitfield.
             * @param bitfield The bitfield to modify.
             * @param n The position of the bit to set.
             */
            inline void SetBit(uint64_t& bitfield, uint8_t n);

            /**
             * @brief Unset a specific bit in a bitfield.
             * @param bitfield The bitfield to modify.
             * @param n The position of the bit to unset.
             */
            inline void UnsetBit(uint64_t& bitfield, uint8_t n);

            /**
             * @brief Check if a specific bit is set in a bitfield.
             * @param bitfield The bitfield to check.
             * @param n The position of the bit to check.
             * @return True if the bit is set, false otherwise.
             */
            inline bool HasBit(uint64_t bitfield, uint8_t n);

            /**
             * @brief Specialization for manipulating individual bits in a 64-bit integer.
             */
            template<class INTEGER>
            struct Bit {};

            /**
             * @brief Specialization for manipulating individual bits in a 64-bit integer.
             */
            template<>
            struct Bit<uint64_t> {
                static constexpr uint64_t Right = 0b0000000000000000000000000000000000000000000000000000000000000001; /**< The rightmost bit. */
            };
        }
    }
}

#include "Utils.inl"
