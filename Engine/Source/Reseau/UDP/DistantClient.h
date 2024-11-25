#include <vector>
#include "Reseau/UDP/Datagram.h"
#include "Reseau/Client/Client.h"
#include "AckHandler.h"

namespace Reseau
{
    namespace UDP
    {
        /**
         * @brief Class representing a distant client in the UDP communication.
         */
        class DistantClient
        {
        public:
            DistantClient() = default;
            DistantClient(const DistantClient&) = delete;
            DistantClient(DistantClient&&) = delete;
            DistantClient& operator=(const DistantClient&) = delete;
            DistantClient& operator=(DistantClient&&) = delete;
            ~DistantClient() {};

            // Methods for handling communication with distant clients
			/*void Send(std::vector<uint8_t>&& data);
			void OnDatagramReceived(Reseau::UDP::Datagram&& datagram);
			void OnDataReceived(std::vector<uint8_t>&& data);
			void OnMessageReady(std::unique_ptr<Reseau::Client::Message>&& msg);
			Reseau::Client::Client GetClient(const sockaddr_storage& clientAddr);*/

        private:
            Reseau::UDP::Datagram::ID mNextDatagramIdToSend{ 0 }; /**< Next datagram ID to send. */
            AckHandler mReceivedAcks; /**< Acknowledgement handler for received datagrams. */
        };
    }
}