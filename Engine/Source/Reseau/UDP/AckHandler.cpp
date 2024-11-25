#include "AckHandler.h"


void Reseau::UDP::AckHandler::Update(uint16_t newAck, uint64_t previousAcks, bool trackLoss)
{
	mLastAckIsNew = false;
	if (newAck == mLastAck)
	{
		//!< Doublon du dernier acquittement, mais le masque peut contenir de nouvelles informations 
		mNewAcks = (mPreviousAcks & previousAcks) ^ previousAcks;
		mPreviousAcks |= previousAcks;
	}
	else if (Reseau::UDP::Utils::IsSequenceNewer(newAck, mLastAck))
	{
		//!< Acquittement plus récent, vérifier les pertes, etc.
		const auto diff = Reseau::UDP::Utils::SequenceDiff(newAck, mLastAck);
		const auto gap = diff - 1;
		//!< Nombre de bits à décaler du masque
		const auto bitsToShift = std::min(diff, static_cast<uint16_t>(64));
		if (trackLoss)
		{
			for (uint32_t i = 0; i < bitsToShift; ++i)
			{
				const auto packetDiffWithLastAck = 64 - i;
				const auto bitInPreviousMask = packetDiffWithLastAck - 1;
				if (!Reseau::UDP::Utils::HasBit(mPreviousAcks, bitInPreviousMask))
				{
					//!< Cet identifiant n’a pas été acquitté et est maintenant hors bornes : marquer comme perdu
					const uint16_t packetid = mLastAck - packetDiffWithLastAck;
					mLoss.push_back(packetid);
				}
			}
		}
		//!< Décaller le masque vers la gauche : supprimer les paquets les plus anciens du masque
		mPreviousAcks <<= bitsToShift;
		if (gap >= 64)
		{
			//!< Il s’agit d’un saut qui supprime entièrement le masque
			mPreviousAcks = mNewAcks = 0;
			//!< Vérifier chaque identifiant du masque pour marquer comme perdus les non-acquittés
			if (trackLoss)
			{
				for (uint32_t p = 64; p < gap; ++p)
				{
					const uint16_t packetid = mLastAck + (p - 64) + 1;
					mLoss.push_back(packetid);
				}
			}
		}
		else
		{
			//!< Marquer l’ancien acquittement comme acquitté dans le masque décalé
			Reseau::UDP::Utils::SetBit(mPreviousAcks, gap);
		}
		mLastAck = newAck;
		mLastAckIsNew = true;
		mNewAcks = (mPreviousAcks & previousAcks) ^ previousAcks;
		mPreviousAcks |= previousAcks;
	}
	else
	{
		//!< Il s’agit d’un vieil acquittement, s’il n’est pas trop dépassé il peut contenir des informations intéressantes
		const auto diff = Reseau::UDP::Utils::SequenceDiff(mLastAck, newAck);
		if (diff <= 64)
		{
			//!< Aligner le masque reçu avec notre masque actuel
			previousAcks <<= diff;
			//!< Insérer l’acquittement dans le masque
			const auto ackBitInMask = diff - 1;
			Reseau::UDP::Utils::SetBit(previousAcks, ackBitInMask);
			//!< Puis mise à jour des acquittements
			mNewAcks = (mPreviousAcks & previousAcks) ^ previousAcks;
			mPreviousAcks |= previousAcks;
		}
	}
}

bool Reseau::UDP::AckHandler::IsAcked(uint16_t ack) const
{
	if (ack == mLastAck)
	{
		return true;
	}
	if (Reseau::UDP::Utils::IsSequenceNewer(ack, mLastAck))
	{
		return false;
	}
	const auto diff = Reseau::UDP::Utils::SequenceDiff(mLastAck, ack);
	if (diff > 64)
	{
		return false;
	}
	const uint8_t bitPosition = static_cast<uint8_t>(diff - 1);
	return Reseau::UDP::Utils::HasBit(mPreviousAcks, bitPosition);
}

bool Reseau::UDP::AckHandler::IsNewlyAcked(uint16_t ack) const
{
	if (ack == mLastAck)
	{
		return mLastAckIsNew;
	}
	if (Utils::IsSequenceNewer(ack, mLastAck))
	{
		return false;
	}
	const auto diff = Reseau::UDP::Utils::SequenceDiff(mLastAck, ack);
	if (diff > 64)
	{
		return false;
	}
	const uint8_t bitPosition = static_cast<uint8_t>(diff - 1);
	return Utils::HasBit(mNewAcks, bitPosition);
}

uint16_t Reseau::UDP::AckHandler::LastAck() const
{
	return mLastAck;
}

uint64_t Reseau::UDP::AckHandler::PreviousAcksMask() const
{
	return mPreviousAcks;
}

std::vector<uint16_t> Reseau::UDP::AckHandler::GetNewAcks() const
{
	std::vector<uint16_t> newAcks;
	newAcks.reserve(65);
	for (uint8_t i = 64; i != 0; --i)
	{
		const uint8_t bitToCheck = i - 1;
		if (Reseau::UDP::Utils::HasBit(mNewAcks, bitToCheck))
		{
			const uint16_t id = mLastAck - i;
			newAcks.push_back(id);
		}
	}
	if (mLastAckIsNew)
	{
		newAcks.push_back(mLastAck);
	}
	return newAcks;
}

std::vector<uint16_t>& Reseau::UDP::AckHandler::Loss()
{
	return mLoss;
}
