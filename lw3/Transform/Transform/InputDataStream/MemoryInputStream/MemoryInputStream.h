#pragma once
#include "../IInputDataStream.h"
#include <vector>

class MemoryInputStream : public IInputDataStream
{
public:
	MemoryInputStream(std::vector<uint8_t>& data);
	~MemoryInputStream() = default;

	bool IsEOF() const override;
	uint8_t ReadByte() override;
	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;
	void Close() override;

private:
	std::vector<uint8_t>& m_data;
	std::size_t m_pos = 0;
	bool m_closed;
};