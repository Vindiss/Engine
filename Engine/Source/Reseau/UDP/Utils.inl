void Reseau::UDP::Utils::SetBit(uint64_t& bitfield, const uint8_t n)
{
	assert(n < 64);
	bitfield |= (Bit<uint64_t>::Right << n);
}
void Reseau::UDP::Utils::UnsetBit(uint64_t& bitfield, const uint8_t n)
{
	assert(n < 64);
	bitfield &= (~(Bit<uint64_t>::Right << n));
}
bool Reseau::UDP::Utils::HasBit(uint64_t bitfield, const uint8_t n)
{
	assert(n < 64);
	return (bitfield & (Bit<uint64_t>::Right << n)) != 0;
}