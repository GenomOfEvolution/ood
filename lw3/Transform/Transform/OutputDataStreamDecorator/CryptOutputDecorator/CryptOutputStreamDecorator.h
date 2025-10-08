#pragma once
#include "../OutputDataStreamDecorator.h"
#include <memory>
#include <vector>

class CryptOutputStreamDecorator : public OutputDataStreamDecorator
{
public:
	CryptOutputStreamDecorator(std::unique_ptr<IOutputDataStream> stream, int key);

	void WriteByte(uint8_t data) override;
	void WriteBlock(const void* srcData, std::streamsize size) override;
	void Close() override;

private:
	uint8_t CryptByte(uint8_t byte) const;
	std::vector<uint8_t> m_cryptTable;
};
