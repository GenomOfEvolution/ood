#include "CompressOutputStreamDecorator.h"

CompressOutputStreamDecorator::CompressOutputStreamDecorator(std::unique_ptr<IOutputDataStream> stream)
	: OutputDataStreamDecorator(std::move(stream))
{
}

CompressOutputStreamDecorator::~CompressOutputStreamDecorator()
{
	Flush();
}

void CompressOutputStreamDecorator::WriteByte(uint8_t data)
{
	if (m_block.size == 0)
	{
		m_block = 
		{
			.byte = data,
			.size = 1,
		};

		return;
	}

	if (m_block.byte == data && m_block.size < std::numeric_limits<std::uint8_t>::max())
	{
		++m_block.size;
		return;
	}

	Flush();

	m_block = 
	{
		.byte = data,
		.size = 1,
	};
}

void CompressOutputStreamDecorator::WriteBlock(const void* srcData, std::streamsize size)
{
	const auto bytes = static_cast<const std::uint8_t*>(srcData);

	for (std::streamsize i = 0; i < size; i++)
	{
		WriteByte(bytes[i]);
	}
}

void CompressOutputStreamDecorator::Close()
{
	Flush();
}

void CompressOutputStreamDecorator::Flush() const
{
	m_stream->WriteByte(m_block.size);
	m_stream->WriteByte(m_block.byte);
}
