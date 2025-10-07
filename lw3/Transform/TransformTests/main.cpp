#define CATCH_CONFIG_MAIN
#include <filesystem>
#include <string>
#include "../../../catch2/catch.hpp"
#include "../Transform/InputDataStream/FileInputStream/FileInputStream.h"
#include "../Transform/InputDataStream/MemoryInputStream/MemoryInputStream.h"

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

	GIVEN("non existing file path")
	{
		std::string const path = GetTemporaryFilepath("non-existing-file.txt");

		WHEN("creating a stream")
		{
			THEN("it throws an exception")
			{
				REQUIRE_THROWS_AS(FileInputStream(path), std::runtime_error);
			}
		}
	}

	std::filesystem::remove(TEST_FILE_NAME);
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

			THEN("subsequent operations throw stream error")
			{
				REQUIRE_THROWS_AS(stream.ReadByte(), std::ios_base::failure);
			}

			THEN("reading a block doesn`t throws an error")
			{
				std::vector<uint8_t> buffer(1);
				REQUIRE_NOTHROW(stream.ReadBlock(buffer.data(), 1));
			}

			THEN("double close throws logic_error")
			{
				REQUIRE_THROWS_AS(stream.Close(), std::logic_error);
			}
		}
	}
}