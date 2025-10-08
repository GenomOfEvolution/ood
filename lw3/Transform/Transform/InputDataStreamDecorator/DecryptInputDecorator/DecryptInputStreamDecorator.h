#pragma once
#include "../InputDataStreamDecorator.h"
#include <memory>
#include <vector>

class DecryptInputStreamDecorator : public InputDataStreamDecorator
{
public:
	DecryptInputStreamDecorator(std::unique_ptr<IInputDataStream> stream, int key);

	uint8_t ReadByte() override;
	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override;

private:
	uint8_t DecryptByte(uint8_t byte) const;
	std::vector<uint8_t> m_decryptTable;
};
