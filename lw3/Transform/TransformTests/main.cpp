#define CATCH_CONFIG_MAIN
#include <memory>
#include <filesystem>
#include <string>
#include "../../../catch2/catch.hpp"

#include "../Transform/InputDataStream/FileInputStream/FileInputStream.h"
#include "../Transform/InputDataStream/MemoryInputStream/MemoryInputStream.h"
#include "../Transform/InputDataStreamDecorator/DecryptInputDecorator/DecryptInputStreamDecorator.h"
#include "../Transform/OutputDataStream/FileOutputStream/FileOutputStream.h"
#include "../Transform/OutputDataStream/MemoryOutputStream/MemoryOutputStream.h"
#include "../Transform/OutputDataStreamDecorator/CryptOutputDecorator/CryptOutputStreamDecorator.h"

const auto TEST_FILE_NAME = "test_file_input.bin";

std::string GetTemporaryFilepath(const std::string& name)
{
	auto const tempDir = std::filesystem::temp_directory_path();
	auto const result = tempDir / name;

	return result.string();
}

auto CreateTestFile = [&](const std::vector<uint8_t>& data) 
	{
		std::ofstream file(GetTemporaryFilepath(TEST_FILE_NAME), std::ios::binary);
		file.write(reinterpret_cast<const char*>(data.data()), data.size());
		file.close();
	};

std::vector<std::uint8_t> GetFileContents(std::string const& filename)
{
	std::ifstream stream(filename, std::ios_base::in | std::ios_base::binary);
	stream.unsetf(std::ios::skipws);

	const std::istream_iterator<std::uint8_t> begin(stream), end;
	std::vector result(begin, end);

	return result;
}


SCENARIO("Memory Input Stream")
{
	std::vector<uint8_t> testData = { 0x01, 0x02, 0x03, 0x04, 0x05 };

	GIVEN("memory input stream")
	{
		MemoryInputStream stream(testData);
		WHEN("reading bytes one by one")
		{
			THEN("EOF is not reached")
			{
				REQUIRE_FALSE(stream.IsEOF());
			}

			THEN("Bytes are in correct order")
			{
				CHECK(stream.ReadByte() == 0x01);
				CHECK(stream.ReadByte() == 0x02);
				CHECK(stream.ReadByte() == 0x03);
				CHECK(stream.ReadByte() == 0x04);
				CHECK(stream.ReadByte() == 0x05);

				AND_THEN("EOF is reached")
				{
					REQUIRE(stream.IsEOF());
				}
				
				AND_THEN("can`t read any more bytes")
				{
					REQUIRE_THROWS_AS(stream.ReadByte(), std::ios_base::failure);
				}
			}
		}

		WHEN("reading block of bytes ")
		{
			std::vector<uint8_t> buffer(3);

			THEN("correct number of bytes is read")
			{
				std::streamsize bytesRead = stream.ReadBlock(buffer.data(), 3);
				REQUIRE(bytesRead == 3);
				REQUIRE(buffer == std::vector<uint8_t>{ 0x01, 0x02, 0x03 });
			}

			THEN("position advances after reading block")
			{
				stream.ReadBlock(buffer.data(), 3);
				REQUIRE(stream.ReadByte() == 0x04);
			}
		}

		WHEN("reading beyond available data")
		{
			std::vector<uint8_t> buffer(10);

			THEN("only available bytes are read")
			{
				std::streamsize bytesRead = stream.ReadBlock(buffer.data(), 10);
				REQUIRE(bytesRead == 5);
				REQUIRE(stream.IsEOF());
			}
		}

		WHEN("closing the stream")
		{
			stream.Close();

			THEN("subsequent operations throw logic_error")
			{
				REQUIRE_THROWS_AS(stream.IsEOF(), std::logic_error);
				REQUIRE_THROWS_AS(stream.ReadByte(), std::logic_error);

				std::vector<uint8_t> buffer(1);
				REQUIRE_THROWS_AS(stream.ReadBlock(buffer.data(), 1), std::logic_error);
			}

			THEN("double close throws logic_error")
			{
				REQUIRE_THROWS_AS(stream.Close(), std::logic_error);
			}
		}
	}
}

SCENARIO("File Input Stream")
{
	std::vector<uint8_t> testData = { 0x01, 0x02, 0x03, 0x04, 0x05 };
	std::filesystem::remove(GetTemporaryFilepath(TEST_FILE_NAME));

	GIVEN("non existing file path")
	{
		std::string const path = GetTemporaryFilepath(TEST_FILE_NAME);

		WHEN("creating a stream")
		{
			THEN("it throws an exception")
			{
				REQUIRE_THROWS_AS(FileInputStream(path), std::runtime_error);
			}
		}
	}

	GIVEN("existing file")
	{
		CreateTestFile(testData);
		FileInputStream stream(GetTemporaryFilepath(TEST_FILE_NAME));

		WHEN("reading bytes sequentially")
		{
			THEN("stream is initially not at EOF")
			{
				REQUIRE_FALSE(stream.IsEOF());
			}

			THEN("bytes are read correctly in order")
			{
				CHECK(stream.ReadByte() == 0x01);
				CHECK(stream.ReadByte() == 0x02);
				CHECK(stream.ReadByte() == 0x03);
				CHECK(stream.ReadByte() == 0x04);
				CHECK(stream.ReadByte() == 0x05);

				AND_THEN("EOF is reached")
				{
					REQUIRE(stream.IsEOF());
				}

				AND_THEN("can`t read any more bytes")
				{
					REQUIRE_THROWS_AS(stream.ReadByte(), std::ios_base::failure);
				}
			}
		}

		WHEN("reading block of bytes")
		{
			std::vector<uint8_t> buffer(3);

			THEN("correct number of bytes is read")
			{
				std::streamsize bytesRead = stream.ReadBlock(buffer.data(), 3);
				REQUIRE(bytesRead == 3);
				REQUIRE(buffer == std::vector<uint8_t>{ 0x01, 0x02, 0x03 });
			}

			THEN("position advances after reading block")
			{
				stream.ReadBlock(buffer.data(), 3);
				REQUIRE(stream.ReadByte() == 0x04);
			}
		}

		WHEN("reading beyond available data")
		{
			std::vector<uint8_t> buffer(10);

			THEN("only available bytes are read")
			{
				std::streamsize bytesRead = stream.ReadBlock(buffer.data(), 10);
				REQUIRE(bytesRead == 5);
				REQUIRE(stream.IsEOF());
			}
		}

		WHEN("closing the stream")
		{
			stream.Close();

			THEN("subsequent operations throw logic error")
			{
				REQUIRE_THROWS_AS(stream.ReadByte(), std::logic_error);
				REQUIRE_THROWS_AS(stream.IsEOF(), std::logic_error);
				std::vector<uint8_t> buffer(1);
				REQUIRE_THROWS_AS(stream.ReadBlock(buffer.data(), 1), std::logic_error);
			}

			THEN("double close throws logic_error")
			{
				REQUIRE_THROWS_AS(stream.Close(), std::logic_error);
			}
		}
	}
	std::filesystem::remove(TEST_FILE_NAME);
}

SCENARIO("Memory Output Stream")
{
	std::vector<uint8_t> data;
	GIVEN("an memory stream")
	{
		MemoryOutputStream stream(data);
		WHEN("writing couple of bytes")
		{
			stream.WriteByte('h');
			stream.WriteByte('e');
			stream.WriteByte('l');
			stream.WriteByte('l');

			THEN("data vector changes")
			{
				REQUIRE(data == std::vector<uint8_t>{ 'h', 'e', 'l', 'l' });
			}

			AND_WHEN("writing block of bytes")
			{
				std::vector<uint8_t> bytes = { 'o', ' ', 'm', 'e' };
				stream.WriteBlock(bytes.data(), static_cast<std::streamsize>(bytes.size()));

				THEN("data vector becomes populated with new bytes")
				{
					REQUIRE(data == std::vector<std::uint8_t>{ 'h', 'e', 'l', 'l', 'o', ' ', 'm', 'e' });
				}
			}
		}

		WHEN("writing zero bytes")
		{
			std::vector<uint8_t> empty;
			stream.WriteBlock(empty.data(), 0);

			THEN("data remains unchanged")
			{
				REQUIRE(data.empty());
			}
		}

		WHEN("closing a stream")
		{
			stream.Close();

			THEN("all subsequent operations throw logic_error")
			{
				std::vector<uint8_t> bytes = { 'd', 'a', 't', 'a' };
				REQUIRE_THROWS_AS(stream.WriteBlock(bytes.data(), static_cast<std::streamsize>(bytes.size())), std::logic_error);
				REQUIRE_THROWS_AS(stream.WriteByte('a'), std::logic_error);
			}

			AND_THEN("double closing throws logcal error")
			{
				REQUIRE_THROWS_AS(stream.Close(), std::logic_error);
			}
		}
	}
}

SCENARIO("File output stream")
{
	std::filesystem::remove(GetTemporaryFilepath(TEST_FILE_NAME));

	GIVEN("a non-existing file in valid directory")
	{
		std::string const path = GetTemporaryFilepath(TEST_FILE_NAME);

		WHEN("creating a stream")
		{
			THEN("it creates the file and does not throw")
			{
				REQUIRE_NOTHROW(FileOutputStream(path));
				REQUIRE(std::filesystem::exists(path));
			}
		}
	}

	GIVEN("existing empty file")
	{
		std::string const path = GetTemporaryFilepath(TEST_FILE_NAME);
		FileOutputStream stream(path);

		WHEN("writing couple of bytes")
		{
			stream.WriteByte('h');
			stream.WriteByte('e');
			stream.WriteByte('l');
			stream.WriteByte('l');

			THEN("data vector changes")
			{
				REQUIRE(GetFileContents(path) == std::vector<uint8_t>{ 'h', 'e', 'l', 'l' });
			}

			AND_WHEN("writing block of bytes")
			{
				std::vector<uint8_t> bytes = { 'o', ' ', 'm', 'e' };
				stream.WriteBlock(bytes.data(), static_cast<std::streamsize>(bytes.size()));

				THEN("data vector becomes populated with new bytes")
				{
					REQUIRE(GetFileContents(path) == std::vector<std::uint8_t>{ 'h', 'e', 'l', 'l', 'o', ' ', 'm', 'e' });
				}
			}
		}

		WHEN("closing a stream")
		{
			stream.Close();
			THEN("all subsequent operations throw logic_error")
			{
				std::vector<uint8_t> bytes = { 'd', 'a', 't', 'a' };
				REQUIRE_THROWS_AS(stream.WriteBlock(bytes.data(), static_cast<std::streamsize>(bytes.size())), std::logic_error);
				REQUIRE_THROWS_AS(stream.WriteByte('a'), std::logic_error);
			}

			AND_THEN("double closing throws logcal error")
			{
				REQUIRE_THROWS_AS(stream.Close(), std::logic_error);
			}
		}
	}
}

SCENARIO("Crypt / decrypt stream")
{
	GIVEN("an vector of data")
	{
		std::vector<uint8_t> data;

		AND_GIVEN("crypt stream")
		{
			int key = 2281337;
			auto cleanOutputStream = std::make_unique<MemoryOutputStream>(data);
			auto cryptStream = std::make_unique<CryptOutputStreamDecorator>(std::move(cleanOutputStream), key);

			WHEN("encrypting bytes")
			{
				std::vector<std::uint8_t> bytes = { 'h', 'e', 'l', 'l', 'o', '!' };
				for (auto&& byte : bytes)
				{
					cryptStream->WriteByte(byte);
				}

				AND_WHEN("reading them with clean input stream")
				{
					auto cleanInputStream = std::make_unique<MemoryInputStream>(data);

					THEN("data is crypted")
					{
						for (auto&& byte : bytes)
						{
							REQUIRE(byte != cleanInputStream->ReadByte());
						}
					}
				}

				AND_WHEN("reading them with decrypt stream")
				{
					AND_WHEN("with correct key")
					{
						auto cleanInputStream = std::make_unique<MemoryInputStream>(data);
						auto decryptStream = std::make_unique<DecryptInputStreamDecorator>(std::move(cleanInputStream), key);
						THEN("they are equal")
						{
							for (auto&& byte : bytes)
							{
								REQUIRE(byte == decryptStream->ReadByte());
							}
						}
					}

					AND_WHEN("with incorrect key")
					{
						auto cleanInputStream = std::make_unique<MemoryInputStream>(data);
						auto decryptStream = std::make_unique<DecryptInputStreamDecorator>(std::move(cleanInputStream), 228);
						THEN("they are not equal")
						{
							for (auto&& byte : bytes)
							{
								REQUIRE(byte != decryptStream->ReadByte());
							}
						}
					}
					
				}

			}
		}
	}
}