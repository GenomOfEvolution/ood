#include "OutputDataStreamDecorator.h"

OutputDataStreamDecorator::OutputDataStreamDecorator(std::unique_ptr<IOutputDataStream>&& stream)
	: m_stream(std::move(stream))
{
}
