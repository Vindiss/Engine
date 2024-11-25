#include "ReceptionHandler.h"
#include "Reseau/Client/Client.h"


std::unique_ptr<Reseau::Client::Message> Reseau::Client::ReceptionHandler::recv()
{
	assert(mSckt != INVALID_SOCKET);
	int ret = ::recv(mSckt, missingDataStartBuffer(), missingDataLength(), 0);
	if (ret > 0)
	{
		mReceived += ret;
		if (mReceived == mBuffer.size())
		{
			if (mState == State::Data)
			{
				std::unique_ptr<Reseau::Client::UserData> msg = std::make_unique<Reseau::Client::UserData>(std::move(mBuffer));
				startHeaderReception();
				return msg;
			}
			else
			{
				startDataReception();
				//!< si jamais les données sont déjà disponibles elles seront ainsi retournées directement
				return recv();
			}
		}
		return nullptr;
	}
	else if (ret == 0)
	{
		//!< connexion terminée correctement
		return std::make_unique<Reseau::Client::Disconnection>(Reseau::Client::Disconnection::Reason::Disconnected);
	}
	else // ret < 0
	{
		//!< traitement d'erreur
		int error = Reseau::Sockets::Error::GetError();
		if (error == static_cast<int>(Reseau::Sockets::Error::Errors::WOULDBLOCK) || error == static_cast<int>(Reseau::Sockets::Error::Errors::AGAIN))
		{
			return nullptr;
		}
		else
		{
			return std::make_unique<Reseau::Client::Disconnection>(Reseau::Client::Disconnection::Reason::Lost);
		}
	}
}

void  Reseau::Client::ReceptionHandler::startHeaderReception()
{
	startReception(HeaderSize, State::Header);
}

void  Reseau::Client::ReceptionHandler::startDataReception()
{
	assert(mBuffer.size() == sizeof(HeaderType));
	HeaderType networkExpectedDataLength;
	auto lastData = mBuffer.back();
	data = lastData;
	memcpy(&networkExpectedDataLength, mBuffer.data(), sizeof(networkExpectedDataLength));
	const auto expectedDataLength = ntohs(networkExpectedDataLength);
	startReception(expectedDataLength, State::Data);
}

void  Reseau::Client::ReceptionHandler::startReception(unsigned int expectedDataLength, State newState)
{
	mReceived = 0;
	mBuffer.clear();
	mBuffer.resize(expectedDataLength, 0);
	mState = newState;
}

void  Reseau::Client::ReceptionHandler::init(SOCKET sckt)
{
	assert(sckt != INVALID_SOCKET);
	mSckt = sckt;
	startHeaderReception();
}


