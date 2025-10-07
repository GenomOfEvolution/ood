#include "MemoryInputStream.h"

MemoryInputStream::MemoryInputStream(std::vector<uint8_t>& data)
	: m_data(data)
{
    m_closed = false;
}

bool MemoryInputStream::IsEOF() const
{
    if (m_closed) 
    {
        throw std::logic_error("Stream is closed");
    }

	return m_pos >= m_data.size();
}

uint8_t MemoryInputStream::ReadByte()
{
    if (m_closed)
    {
        throw std::logic_error("Stream is closed");
    }

    if (IsEOF())
    {
        throw std::ios_base::failure("End of stream");
    }

    return m_data[m_pos++];
}

std::streamsize MemoryInputStream::ReadBlock(void* dstBuffer, std::streamsize size)
{
    if (m_closed)
    {
        throw std::logic_error("Stream is closed");
    }

    if (IsEOF()) 
    {
        return 0;
    }

    auto available = m_data.size() - m_pos;
    auto toRead = std::min(static_cast<size_t>(size), available);

    if (toRead > 0)
    {
        memcpy(dstBuffer, m_data.data() + m_pos, toRead);
        m_pos += toRead;
    }

    return static_cast<std::streamsize>(toRead);
}

void MemoryInputStream::Close()
{
    if (m_closed)
    {
        throw std::logic_error("Stream is already closed");
    }

    m_closed = true;
}
