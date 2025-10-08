#include "InputDataStreamDecorator.h"

bool InputDataStreamDecorator::IsEOF() const
{
	return m_stream->IsEOF();
}

void InputDataStreamDecorator::Close()
{
	m_stream->Close();
}

InputDataStreamDecorator::InputDataStreamDecorator(std::unique_ptr<IInputDataStream>&& stream)
	: m_stream(std::move(stream))
{
}
