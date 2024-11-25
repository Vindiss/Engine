#include "ClientImpl.h"


Reseau::Client::ClientImpl::~ClientImpl()
{
	Disconnect();
}

bool Reseau::Client::ClientImpl::Init(SOCKET&& sckt, const sockaddr_in& addr)
{
	assert(sckt != INVALID_SOCKET);
	if (sckt == INVALID_SOCKET)
	{
		return false;
	}

	assert(mState == State::Disconnected);
	assert(mSocket == INVALID_SOCKET);
	if (mSocket != INVALID_SOCKET)
	{
		Disconnect();
	}

	mSocket = sckt;
	if (Reseau::Sockets::Socket::SetNonBlocking(mSocket) != 0)
	{
		Disconnect();
		return false;
	}
	OnConnected(addr);
	return true;
}

bool Reseau::Client::ClientImpl::Connect(const std::string& ipaddress, unsigned short port)
{
	assert(mState == State::Disconnected);
	assert(mSocket == INVALID_SOCKET);
	if (mSocket != INVALID_SOCKET)
		Disconnect();
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		return false;
	}
	else if (!Reseau::Sockets::Socket::SetNonBlocking(mSocket))
	{
		Disconnect();
		return false;
	}
	if (mConnectionHandler.connect(mSocket, ipaddress, port))
	{
		mState = State::Connecting;
		return true;
	}
	return false;
}

void Reseau::Client::ClientImpl::Disconnect()
{
	if (mSocket != INVALID_SOCKET)
	{
		Reseau::Sockets::Socket::CloseSocket(mSocket);
	}
	mSocket = INVALID_SOCKET;
	memset(&mAddress, 0, sizeof(mAddress));
	mState = State::Disconnected;
}

bool Reseau::Client::ClientImpl::Send(const unsigned char* data, unsigned int len)
{
	msgChat = reinterpret_cast <char>(data);
	return mSendingHandler.Send(data, len);
}

std::unique_ptr<Reseau::Client::Message> Reseau::Client::ClientImpl::poll()
{
	switch (mState)
	{
		case State::Connecting:
		{
			auto msg = mConnectionHandler.poll();
			if (msg)
			{
				if (msg->result == Reseau::Client::Connection::Result::Success)
				{
					OnConnected(mConnectionHandler.getConnectedAddress());
				}
				else
				{
					Disconnect();
				}
			}
			return msg;
		} break;
		case State::Connected:
		{
			mSendingHandler.Update();
			auto msg = mReceivingHandler.recv();
			msgChat = mReceivingHandler.GetData();
			if (msg && msg->is<Reseau::Client::Disconnection>())
			{
				Disconnect();
			}
			return msg;
		} break;
		case State::Disconnected:
		{
		} break;
	}
	return nullptr;
}

void Reseau::Client::ClientImpl::OnConnected(const sockaddr_in& addr)
{
	mAddress = addr;
	mSendingHandler.init(mSocket);
	mReceivingHandler.init(mSocket);
	mState = State::Connected;
}
