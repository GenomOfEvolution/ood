#include "Transformer.h"
#include "../InputDataStream/FileInputStream/FileInputStream.h"
#include "../OutputDataStream/FileOutputStream/FileOutputStream.h"
#include "../OutputDataStreamDecorator/CryptOutputDecorator/CryptOutputStreamDecorator.h"
#include "../OutputDataStreamDecorator/CompressOutputDecorator/CompressOutputStreamDecorator.h"
#include "../InputDataStreamDecorator/DecryptInputDecorator/DecryptInputStreamDecorator.h"
#include "../InputDataStreamDecorator/DecompressInputDecorator/DecompressInputStreamDecorator.h"

Transformer::Transformer(const Args& args)
	: m_input(std::make_unique<FileInputStream>(args.inputFileName))
	, m_output(std::make_unique<FileOutputStream>(args.outputFileName))
{
	ApplyDecorators(args);
}

void Transformer::Transform()
{
	while (!m_input->IsEOF())
	{
		m_output->WriteByte(m_input->ReadByte());
	}

	m_output->Close();
	m_input->Close();
}

void Transformer::ApplyDecorators(const Args& args)
{
	for (auto i : args.cryptKeys)
	{
		m_output = std::move(std::make_unique<CryptOutputStreamDecorator>(std::move(m_output), i));
	}

	for (auto i : args.decryptKeys)
	{
		m_input = std::move(std::make_unique<DecryptInputStreamDecorator>(std::move(m_input), i));
	}

	if (args.compress)
	{
		m_output = std::move(std::make_unique<CompressOutputStreamDecorator>(std::move(m_output)));
	}

	if (args.decompress)
	{
		m_input = std::move(std::make_unique<DecompressInputStreamDecorator>(std::move(m_input)));
	}
}
