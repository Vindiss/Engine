#include "Utils.h"


bool Reseau::UDP::Utils::IsSequenceNewer(uint16_t sNew, uint16_t sLast)
{
	if (sNew == sLast)
	{
		return false;
	}
	return (sNew > sLast && sNew - sLast <= std::numeric_limits<uint16_t>::max() / 2) // cas simple : 4 > 2
		|| (sNew < sLast && sLast - sNew > std::numeric_limits<uint16_t>::max() / 2); // dépassement : 2 > 65534
}

uint16_t Reseau::UDP::Utils::SequenceDiff(uint16_t sNew, uint16_t sLast)
{
	if (sNew == sLast)
	{
		return 0;
	}
	assert(IsSequenceNewer(sNew, sLast));
	if (sNew > sLast && sNew - sLast <= std::numeric_limits<uint16_t>::max() / 2)
	{
		return sNew - sLast;
	}
	//!< dépassement
	return (std::numeric_limits<uint16_t>::max() - sLast) + sNew + 1; //!< +1 pour compter le 0 : si sLast == sMax && sNew == 0, la difference est 1
}