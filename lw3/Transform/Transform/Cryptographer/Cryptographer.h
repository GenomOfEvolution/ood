#pragma once
#include <vector>

class Cryptographer
{
public:
	static std::vector<uint8_t> BuildCryptTable(int key);
	static std::vector<uint8_t> BuildDecryptTable(int key);
};