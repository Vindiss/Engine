#include "SendingHandler.h"
#include "Reseau/Client/Client.h"


bool Reseau::Client::SendingHandler::Send(const unsigned char* data, unsigned int datalen)
{
	if (datalen > (unsigned int)std::numeric_limits<HeaderType>::max)
	{
		return false;
	}
	mQueueingBuffers.emplace_back(data, data + datalen);
	return true;
}

void Reseau::Client::SendingHandler::Update()
{
	assert(mSocket != INVALID_SOCKET);
	if (mState == State::Idle && !mQueueingBuffers.empty())
	{
		prepareNextHeader();
	}
	while (mState != State::Idle && sendPendingBuffer())
	{

		if (mState == State::Header)
		{
			prepareNextData();
		}
		else
		{
			if (!mQueueingBuffers.empty())
			{
				prepareNextHeader();
			}
			else
			{
				mState = State::Idle;
			}
		}
	}
}

size_t Reseau::Client::SendingHandler::QueueSize() const
{
	size_t s = std::accumulate(mQueueingBuffers.cbegin(), mQueueingBuffers.cend(), static_cast<size_t>(0), [](size_t n, const std::vector<unsigned char>& queuedItem) {return n + queuedItem.size() + HeaderSize;});
	if (mState == State::Data)
	{
		s += mSendingBuffer.size();
	}
	return s;
}

bool Reseau::Client::SendingHandler::sendPendingBuffer()
{
	if (mSendingBuffer.empty())
	{
		return true;
	}
		
	//!< envoi des données restantes du dernier envoi
	int sent = ::send(mSocket, reinterpret_cast<char*>(mSendingBuffer.data()), static_cast<int>(mSendingBuffer.size()), 0);
	if (sent > 0)
	{
		if (sent == mSendingBuffer.size())
		{
			//!< toutes les données ont été envoyées
			mSendingBuffer.clear();
			return true;
		}
		else
		{
			//!< envoi partiel
			memmove(mSendingBuffer.data() + sent, mSendingBuffer.data(), sent);
			mSendingBuffer.erase(mSendingBuffer.cbegin() + sent, mSendingBuffer.cend());
		}
	}
	return false;
}

void Reseau::Client::SendingHandler::prepareNextHeader()
{
	assert(!mQueueingBuffers.empty());
	auto header = static_cast<HeaderType>(mQueueingBuffers.front().size());
	const auto networkHeader = htons(header);
	mSendingBuffer.clear();
	mSendingBuffer.resize(HeaderSize);
	memcpy(mSendingBuffer.data(), &networkHeader, sizeof(HeaderType));
	mState = State::Header;
}

void Reseau::Client::SendingHandler::prepareNextData()
{
	assert(!mQueueingBuffers.empty());
	mSendingBuffer.swap(mQueueingBuffers.front());
	mQueueingBuffers.pop_front();
	mState = State::Data;
}

void Reseau::Client::SendingHandler::init(SOCKET sckt)
{
	mSocket = sckt;
	if (mState == State::Header || mState == State::Data)
	{
		mSendingBuffer.clear();
	}
	mState = State::Idle;
}