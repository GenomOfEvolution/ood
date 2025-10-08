#include "DecompressInputStreamDecorator.h"

DecompressInputStreamDecorator::DecompressInputStreamDecorator(std::unique_ptr<IInputDataStream> stream)
	: InputDataStreamDecorator(std::move(stream))
{
}

uint8_t DecompressInputStreamDecorator::ReadByte()
{
	if (m_block.size == 0)
	{
		m_block.size = m_stream->ReadByte();
		m_block.byte = m_stream->ReadByte();
	}

	--m_block.size;
	return m_block.byte;
}

std::streamsize DecompressInputStreamDecorator::ReadBlock(void* dstBuffer, std::streamsize size)
{
	auto bytes = static_cast<std::uint8_t*>(dstBuffer);

	std::streamsize i;
	for (i = 0; i < size && !IsEOF(); ++i)
	{
		bytes[i] = ReadByte();
	}

	return i + 1;
}
