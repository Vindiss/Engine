#include "ConnectionHandler.h"

bool Reseau::Client::ConnectionHandler::connect(SOCKET sckt, const std::string& address, unsigned short port)
{
	assert(sckt != INVALID_SOCKET);
	mAddress = address;
	mPort = port;
	mFd.fd = sckt;
	mFd.events = POLLOUT;
	inet_pton(AF_INET, mAddress.c_str(), &mConnectedAddress.sin_addr.s_addr);
	mConnectedAddress.sin_family = AF_INET;
	mConnectedAddress.sin_port = htons(mPort);
	if (::connect(sckt, (const sockaddr*)&mConnectedAddress, sizeof(mConnectedAddress)) != 0)
	{
		int err = Reseau::Sockets::Error::GetError();
		if (err != static_cast<int>(Reseau::Sockets::Error::Errors::INPROGRESS) && err != static_cast<int>(Reseau::Sockets::Error::Errors::WOULDBLOCK))
			return false;
	}
	return true;
}

std::unique_ptr<Reseau::Client::Connection> Reseau::Client::ConnectionHandler::poll()
{
	int res = ::poll(&mFd, 1, 0);
	if (res < 0)
		return std::make_unique<Reseau::Client::Connection>(Reseau::Client::Connection::Result::Failed);
	else if (res > 0)
	{
		if (mFd.revents & POLLOUT)
		{
			return std::make_unique <Reseau::Client::Connection> (Reseau::Client::Connection::Result::Success);
		}
		else if (mFd.revents & (POLLHUP | POLLNVAL))
		{
			return std::make_unique<Reseau::Client::Connection>(Reseau::Client::Connection::Result::Failed);
		}
		else if (mFd.revents & POLLERR)
		{
			return std::make_unique<Reseau::Client::Connection>(Reseau::Client::Connection::Result::Failed);
		}
	}
	return nullptr;
}
