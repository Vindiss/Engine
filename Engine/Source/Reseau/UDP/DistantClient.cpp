//#include "DistantClient.h"
//
//void Reseau::UDP::DistantClient::Send(std::vector<uint8_t>&& data)
//{
//	Datagram datagram;
//	datagram.header.id = htons(mNextDatagramIdToSend);
//	datagram.header.ack = htons(mReceivedAcks.LastAck());
//	datagram.header.previousAcks = mReceivedAcks.PreviousAcksMask();
//	++mNextDatagramIdToSend;
//	memcpy(datagram.data.data(), data.data(), data.size());
//
//	sendto(mClient.mSocket, reinterpret_cast<const char*>(&datagram), static_cast<int>(Reseau::UDP::Datagram::HeaderSize + data.size()), 0, reinterpret_cast<const sockaddr*>(&mAddress), sizeof(mAddress));
//}
//
//void Reseau::UDP::DistantClient::OnDatagramReceived(Reseau::UDP::Datagram&& datagram)
//{
//	const auto datagramid = ntohs(datagram.header.id);
//	//!< Mise à jour des datagrammes reçus
//	mReceivedAcks.Update(datagramid, datagram.header.previousAcks, true);
//	//!< Ignorer les duplications
//	if (!mReceivedAcks.IsNewlyAcked(datagramid))
//	{
//		return;
//	}
//
//	mReceivedAcks.Update(ntohs(datagram.header.ack), datagram.header.previousAcks, true);
//	const auto lostDatagrams = mReceivedAcks.Loss();
//	for (const auto receivedLostDatagram : lostDatagrams)
//	{
//		//onDatagramReceivedLost(receivedLostDatagram);
//	}
//	//!< Gérer les données envoyées et non reçues
//	const auto datagramsSentLost = mReceivedAcks.Loss();
//	for (const auto sendLoss : datagramsSentLost)
//	{
//		//onDatagramSentLost(sendLoss);
//	}
//	//!< Traiter les données envoyées et acquittées
//	const auto datagramsSentAcked = mReceivedAcks.GetNewAcks();
//	for (const auto sendAcked : datagramsSentAcked)
//	{
//		//onDatagramSentAcked(sendAcked);
//	}
//
//	//!< Gérer les données reçues
//	OnDataReceived(std::vector<uint8_t>(datagram.data.data(), datagram.data.data() + datagram.datasize));
//}
//
//void Reseau::UDP::DistantClient::OnDataReceived(std::vector<uint8_t>&& data)
//{
//	OnMessageReady(std::make_unique<Reseau::Client::UserData>(std::move(data)));
//}
//
//void Reseau::UDP::DistantClient::OnMessageReady(std::unique_ptr<Reseau::Client::Message>&& msg)
//{
//	memcpy(&(msg->from), &mAddress, sizeof(mAddress));
//	mClient.onMessageReceived(std::move(msg));
//}
//
//Reseau::Client::Client Reseau::UDP::DistantClient::GetClient(const sockaddr_storage& clientAddr)
//{
//	auto itClient = std::find_if(mClients.begin(), mClients.end(), [&](const std::unique_ptr<DistantClient>& client) { return memcmp(&(client->address()), &clientAddr, sizeof(sockaddr_storage)) == 0; });
//	if (itClient != mClients.end())
//	{
//		return *(itClient->get());
//	}
//
//	mClients.emplace_back(std::make_unique<DistantClient>(*this, clientAddr));
//	return *(mClients.back());
//}
