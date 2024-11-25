#include "Reseau/ServeurImpl.h"

Reseau::ServeurImpl::~ServeurImpl()
{
	Stop();
}

bool Reseau::ServeurImpl::Start(unsigned short _port)
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	assert(mSocket == INVALID_SOCKET);
	if (mSocket != INVALID_SOCKET)
	{
		Stop();
	}
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		Reseau::Sockets::Error::GetError();
		return false;
	}
	if (Reseau::Sockets::Socket::SetReuseAddr(mSocket) != 0 || Reseau::Sockets::Socket::SetNonBlocking(mSocket) != 0)
	{
		Stop();
		return false;
	}

	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	addr.sin_family = AF_INET;
	if (bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
	{
		Stop();
		return false;
	}
	if (listen(mSocket, SOMAXCONN) != 0)
	{
		Stop();
		return false;
	}
	return true;
}

void Reseau::ServeurImpl::Stop()
{
	for (auto& client : mClients)
	{
		client.second.disconnect();
	}
	mClients.clear();
	if (mSocket != INVALID_SOCKET)
	{
		Reseau::Sockets::Socket::CloseSocket(mSocket);
	}
	mSocket = INVALID_SOCKET;
}

void Reseau::ServeurImpl::Update()
{
	if (mSocket == INVALID_SOCKET)
	{
		return;
	}

	//!< accept jusqu'à 10 nouveaux clients
	for (int accepted = 0; accepted < 10; ++accepted)
	{
		sockaddr_in addr = { 0 };
		socklen_t addrlen = sizeof(addr);
		SOCKET newClientSocket = accept(mSocket, reinterpret_cast<sockaddr*>(&addr), &addrlen);
		Reseau::Sockets::Error::GetError();
		if (newClientSocket == INVALID_SOCKET)
		{
			break;
		}
		Reseau::Client::Client newClient;
		if (newClient.init(std::move(newClientSocket), addr))
		{
			auto message = std::make_unique<Reseau::Client::Connection>(Reseau::Client::Connection::Result::Success);
			message->idFrom = newClient.id();
			message->from = newClient.destinationAddress();
			mMessages.push_back(std::move(message));
			mClients[newClient.id()] = std::move(newClient);
		}
	}

	for (auto itClient = mClients.begin(); itClient != mClients.end(); )
	{
		auto& client = itClient->second;
		auto msg = client.poll();
		if (msg)
		{
			msg->from = itClient->second.destinationAddress();
			msg->idFrom = itClient->second.id();
			if (msg->is<Reseau::Client::Disconnection>())
			{
				itClient = mClients.erase(itClient);
			}
			else
			{
				++itClient;
			}
			mMessages.push_back(std::move(msg));
		}
		else
		{
			++itClient;
		}
	}
}

std::unique_ptr<Reseau::Client::Message> Reseau::ServeurImpl::poll()
{
	if (mMessages.empty())
	{
		return nullptr;
	}
	auto msg = std::move(mMessages.front());
	mMessages.pop_front();
	return msg;
}

bool Reseau::ServeurImpl::SendTo(uint64_t clientid, const unsigned char* data, unsigned int len)
{
	auto itClient = mClients.find(clientid);
	msgChat = reinterpret_cast <char>(data);
	return itClient != mClients.end() && itClient->second.send(data, len);
}

bool Reseau::ServeurImpl::SendToAll(const unsigned char* data, unsigned int len)
{
	bool ret = true;
	msgChat = reinterpret_cast <char>(data);
	for (auto& client : mClients)
	{
		ret &= client.second.send(data, len);
	}
	return ret;
}
