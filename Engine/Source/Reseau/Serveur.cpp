#include "Serveur.h"

Reseau::Serveur::Serveur(){}

Reseau::Serveur::~Serveur(){}

Reseau::Serveur::Serveur(Serveur&& other) : mImpl(std::move(other.mImpl)){}

Reseau::Serveur& Reseau::Serveur::operator=(Serveur&& other)
{
	mImpl = std::move(other.mImpl);
	return *this;
}

bool Reseau::Serveur::Start(unsigned short _port)
{
	if (!mImpl)
	{
		mImpl = std::make_unique<Reseau::ServeurImpl>();
	}
	return mImpl && mImpl->Start(_port);
}

void Reseau::Serveur::Uptade()
{
	if (mImpl)
	{
		mImpl->Update();
	}
}

void Reseau::Serveur::Stop()
{
	if (mImpl)
	{
		mImpl->Stop();
	}
}

bool Reseau::Serveur::SendTo(uint64_t clientid, const unsigned char* data, unsigned int len)
{
	return mImpl && mImpl->SendTo(clientid, data, len);
}

bool Reseau::Serveur::SendToAll(const unsigned char* data, unsigned int len)
{
	return mImpl && mImpl->SendToAll(data, len);
}

std::unique_ptr<Reseau::Client::Message> Reseau::Serveur::poll()
{
	return mImpl ? mImpl->poll() : nullptr;
}

char Reseau::Serveur::GetMessageChat()
{
	return mImpl && mImpl->GetMessageChat();
}
