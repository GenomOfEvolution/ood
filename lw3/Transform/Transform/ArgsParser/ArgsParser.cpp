#include "ArgsParser.h"
#include <iostream>
#include <stdexcept>

ArgsParser::ArgsParser(int argc, char* argv[])
    : m_actionMap({
        {"--encrypt",    [this](const std::string& param, Args& args) { ApplyKey(param, args.cryptKeys, "--encrypt"); }},
        {"--decrypt",    [this](const std::string& param, Args& args) { ApplyKey(param, args.decryptKeys, "--decrypt"); }},
        {"--compress",   [this](const std::string& param, Args& args) { ApplyCompress(param, args); }},
        {"--decompress", [this](const std::string& param, Args& args) { ApplyDecompress(param, args); }},
        })
{
    if (argc < 3)
    {
        throw std::runtime_error("Usage: transform.exe [options] <input-file> <output-file>");
    }

    for (int i = 1; i < argc; i++)
    {
        m_argv.push_back(argv[i]);
    }
}

Args ArgsParser::GetArgs() const
{
    Args result;
    size_t i = 0;

    while (i < m_argv.size())
    {
        const std::string& arg = m_argv[i];
        auto handler = m_actionMap.find(arg);

        if (handler != m_actionMap.end())
        {
            std::string param = "";
            if (arg == "--encrypt" || arg == "--decrypt")
            {
                if (i + 1 >= m_argv.size())
                {
                    throw std::runtime_error("Missing parameter for option: " + arg);
                }

                param = m_argv[i + 1];
                i += 2; 
            }
            else
            {
                i += 1; 
            }

            handler->second(param, result);
        }
        else
        {
            if (result.inputFileName.empty())
            {
                result.inputFileName = arg;
            }
            else if (result.outputFileName.empty())
            {
                result.outputFileName = arg;
            }
            else
            {
                throw std::runtime_error("Unexpected argument: " + arg);
            }

            i++;
        }
    }

    if (result.inputFileName.empty())
    {
        throw std::runtime_error("Input file name is required");
    }

    if (result.outputFileName.empty())
    {
        throw std::runtime_error("Output file name is required");
    }

    return result;
}

void ArgsParser::ApplyKey(const std::string& param, std::vector<int>& keys, const std::string& optionName)
{
    if (param.empty())
    {
        throw std::runtime_error("Key is required for " + optionName);
    }

    try
    {
        int key = std::stoi(param);
        keys.push_back(key);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("Invalid key for " + optionName + ": " + param);
    }
}

void ArgsParser::ApplyCompress(const std::string& param, Args& args)
{
    args.compress = true;
}

void ArgsParser::ApplyDecompress(const std::string& param, Args& args)
{
    args.decompress = true;
}