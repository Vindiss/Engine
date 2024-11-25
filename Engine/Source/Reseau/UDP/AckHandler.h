#pragma once

#include <cstdint>
#include <vector>
#include "Utils.h"

namespace Reseau
{
    namespace UDP
    {
        /**
         * @brief Acknowledgment handler for the UDP protocol.
         */
        class AckHandler
        {
        public:
            /**
             * @brief Default constructor.
             */
            AckHandler() = default;

            /**
             * @brief Default copy constructor.
             */
            AckHandler(const AckHandler&) = default;

            /**
             * @brief Default assignment operator.
             */
            AckHandler& operator=(const AckHandler&) = default;

            /**
             * @brief Default move constructor.
             */
            AckHandler(AckHandler&&) = default;

            /**
             * @brief Default move assignment operator.
             */
            AckHandler& operator=(AckHandler&&) = default;

            /**
             * @brief Default destructor.
             */
            ~AckHandler() = default;

            /**
             * @brief Updates the acknowledgment with the new acknowledgment number.
             *
             * @param newAck The new acknowledgment number.
             * @param previousAcks The mask of previous acknowledgments.
             * @param trackLoss Indicates whether to track losses.
             */
            void Update(uint16_t newAck, uint64_t previousAcks, bool trackLoss = false);

            /**
             * @brief Checks if an acknowledgment number has already been received.
             *
             * @param ack The acknowledgment number to check.
             * @return true if the acknowledgment is received, false otherwise.
             */
            bool IsAcked(uint16_t ack) const;

            /**
             * @brief Checks if an acknowledgment number is newly received.
             *
             * @param ack The acknowledgment number to check.
             * @return true if the acknowledgment is newly received, false otherwise.
             */
            bool IsNewlyAcked(uint16_t ack) const;

            /**
             * @brief Returns the last received acknowledgment number.
             *
             * @return The last received acknowledgment number.
             */
            uint16_t LastAck() const;

            /**
             * @brief Returns the mask of previous acknowledgments.
             *
             * @return The mask of previous acknowledgments.
             */
            uint64_t PreviousAcksMask() const;

            /**
             * @brief Returns the new acknowledgments.
             *
             * @return A vector containing the new acknowledgments.
             */
            std::vector<uint16_t> GetNewAcks() const;

            /**
             * @brief Returns the lost acknowledgment numbers.
             *
             * @return A vector containing the lost acknowledgment numbers.
             */
            std::vector<uint16_t>& Loss();

        private:
            uint16_t mLastAck; /**< The last received acknowledgment number. */
            uint64_t mPreviousAcks; /**< The mask of previous acknowledgments. */
            uint64_t mNewAcks{ 0 }; /**< The new acknowledgments. */
            std::vector<uint16_t> mLoss; /**< The lost acknowledgment numbers. */
            bool mLastAckIsNew{ false }; /**< Indicates whether the last acknowledgment is new. */
        };
    }
}
