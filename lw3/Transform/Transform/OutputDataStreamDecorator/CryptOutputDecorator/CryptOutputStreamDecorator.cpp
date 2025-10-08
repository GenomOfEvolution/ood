#include "CryptOutputStreamDecorator.h"
#include "../../Cryptographer/Cryptographer.h"

CryptOutputStreamDecorator::CryptOutputStreamDecorator(std::unique_ptr<IOutputDataStream> stream, int key)
	: OutputDataStreamDecorator(std::move(stream))
	, m_cryptTable(Cryptographer::BuildCryptTable(key))
{
}

void CryptOutputStreamDecorator::WriteByte(uint8_t data)
{
	m_stream->WriteByte(CryptByte(data));
}

void CryptOutputStreamDecorator::WriteBlock(const void* srcData, std::streamsize size)
{
	const auto bytes = static_cast<const uint8_t*>(srcData);

	for (std::streamsize i = 0; i < size; ++i)
	{
		WriteByte(bytes[i]);
	}
}

void CryptOutputStreamDecorator::Close()
{
	m_stream->Close();
}

uint8_t CryptOutputStreamDecorator::CryptByte(uint8_t byte) const
{
	return m_cryptTable[byte];
}