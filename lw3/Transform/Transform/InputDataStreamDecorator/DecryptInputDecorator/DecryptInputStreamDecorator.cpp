#include "DecryptInputStreamDecorator.h"
#include "../../Cryptographer/Cryptographer.h"

DecryptInputStreamDecorator::DecryptInputStreamDecorator(std::unique_ptr<IInputDataStream> stream, int key)
	: InputDataStreamDecorator(std::move(stream))
	, m_decryptTable(Cryptographer::BuildDecryptTable(key))
{
}

uint8_t DecryptInputStreamDecorator::ReadByte()
{
	return DecryptByte(m_stream->ReadByte());
}

std::streamsize DecryptInputStreamDecorator::ReadBlock(void* dstBuffer, std::streamsize size)
{
	std::streamsize readSize = m_stream->ReadBlock(dstBuffer, size);
	auto bytes = static_cast<uint8_t*>(dstBuffer);

	for (int i = 0; i < readSize; i++)
	{
		bytes[i] = DecryptByte(bytes[i]);
	}

	return readSize;
}

uint8_t DecryptInputStreamDecorator::DecryptByte(uint8_t byte) const
{
	return m_decryptTable[byte];
}
