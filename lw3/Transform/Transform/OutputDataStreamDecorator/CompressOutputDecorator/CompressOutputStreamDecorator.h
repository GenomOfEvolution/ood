#pragma once
#include "../OutputDataStreamDecorator.h"
#include "../../InputDataStreamDecorator/DecompressInputDecorator/DecompressInputStreamDecorator.h"

class CompressOutputStreamDecorator : public OutputDataStreamDecorator
{
public:
	CompressOutputStreamDecorator(std::unique_ptr<IOutputDataStream> stream);
	~CompressOutputStreamDecorator();
	
	void WriteByte(uint8_t data) override;
	void WriteBlock(const void* srcData, std::streamsize size) override;
	void Close() override;

private:
	void Flush() const;
	Block m_block;
};
