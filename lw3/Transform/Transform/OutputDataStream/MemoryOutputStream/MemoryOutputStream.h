#pragma once
#include "../IOutputDataStream.h"
#include <vector>

class MemoryOutputStream : public IOutputDataStream
{
public:
	MemoryOutputStream(std::vector<uint8_t>& data);
	
	void WriteByte(uint8_t data) override;
	void WriteBlock(const void* srcData, std::streamsize size) override;
	void Close() override;

private:
	std::vector<uint8_t>& m_data;
	bool m_closed = false;
};
