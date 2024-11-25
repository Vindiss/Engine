#include "Client.h"

Reseau::Client::Client::Client(){}

Reseau::Client::Client::~Client(){}

Reseau::Client::Client::Client(Client&& other) : mImpl(std::move(other.mImpl)){}

Reseau::Client::Client& Reseau::Client::Client::operator=(Client&& other) { mImpl = std::move(other.mImpl); return *this; }

bool Reseau::Client::Client::init(SOCKET&& sckt, const sockaddr_in& addr)
{
	if (!mImpl)
	{
		mImpl = std::make_unique<ClientImpl>();
	}
	return mImpl && mImpl->Init(std::move(sckt), addr);
}

//bool Reseau::Client::Client::init(uint16_t port)
//{
//	//Release();
//	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (mSocket == INVALID_SOCKET)
//	{
//		return false;
//	}
//
//	sockaddr_in addr;
//	addr.sin_addr.s_addr = INADDR_ANY;
//	addr.sin_port = htons(port);
//	addr.sin_family = AF_INET;
//	int res = bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
//	if (res != 0)
//	{
//		return false;
//	}
//
//	if (!Reseau::Sockets::Socket::SetNonBlocking(mSocket))
//	{
//		return false;
//	}
//	return true;
//}

bool Reseau::Client::Client::connect(const std::string& ipaddress, unsigned short port)
{
	if (!mImpl)
	{
		mImpl = std::make_unique<ClientImpl>();
	}
	return mImpl && mImpl->Connect(ipaddress, port);
}

void Reseau::Client::Client::disconnect()
{ 
	if (mImpl != NULL)
	{
		mImpl->Disconnect(); mImpl.reset();
	}
}

bool Reseau::Client::Client::send(const unsigned char* data, unsigned int len)
{
	return mImpl && mImpl->Send(data, len);
}

//void Reseau::Client::Client::SendTo(const sockaddr_storage& target, std::vector<uint8_t>&& data)
//{
//	auto& client = Reseau::UDP::DistantClient::GetClient(target);
//	client.send(std::move(data));
//}

std::unique_ptr<Reseau::Client::Message> Reseau::Client::Client::poll()
{
	return mImpl ? mImpl->poll() : nullptr;
}

char Reseau::Client::Client::GetMessageChat()
{
	return mImpl && mImpl->GetMessageChat();
}

uint64_t Reseau::Client::Client::id() const
{
	return mImpl ? mImpl->Id() : 0xffffffffffffffff;
}

const sockaddr_in& Reseau::Client::Client::destinationAddress() const
{
	static sockaddr_in empty{ 0 }; return mImpl ? mImpl->DestinationAddress() : empty;
}

//void Reseau::Client::Client::OnMessageReceived(std::unique_ptr<Reseau::Client::Message>&& msg)
//{
//	mMessages.push_back(std::move(msg));
//}

//void Reseau::Client::Client::Receive()
//{
//	for (;;)
//	{
//		Reseau::UDP::Datagram datagram;
//		sockaddr_in from{ 0 };
//		socklen_t fromlen = sizeof(from);
//		int ret = recvfrom(mSocket, reinterpret_cast<char*>(&datagram), Reseau::UDP::Datagram::BufferMaxSize, 0, reinterpret_cast<sockaddr*>(&from), &fromlen);
//		if (ret > 0)
//		{
//			if (ret > Reseau::UDP::Datagram::HeaderSize)
//			{
//				datagram.datasize = ret - Reseau::UDP::Datagram::HeaderSize;
//				auto& client = Reseau::UDP::DistantClient::GetClient(reinterpret_cast<sockaddr_storage&>(from));
//				client.onDatagramReceived(std::move(datagram));
//			}
//		}
//		else
//		{
//			if (ret < 0)
//			{
//				//!< Gestion des erreurs
//				const auto err = Reseau::Sockets::Error::GetError();
//				if (err != static_cast<int>(Reseau::Sockets::Error::Errors::WOULDBLOCK))
//				{
//					OutputDebugString(L"erreur");
//				}
//			}
//			return;
//		}
//	}
//}
