#pragma once
#include "../InputDataStreamDecorator.h"

struct Block
{
	std::uint8_t byte = 0;
	std::uint8_t size = 0;
};

class DecompressInputStreamDecorator : public InputDataStreamDecorator
{
public:
	DecompressInputStreamDecorator(std::unique_ptr<IInputDataStream> stream);

	uint8_t ReadByte() override;
	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;

private:
	Block m_block;
};
