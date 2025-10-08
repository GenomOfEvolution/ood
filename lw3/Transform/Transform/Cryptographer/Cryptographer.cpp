#include "Cryptographer.h"
#include <random>

std::vector<uint8_t> Cryptographer::BuildCryptTable(int key)
{
    std::vector<uint8_t> table(256);

    for (int i = 0; i < 256; i++)
    {
        table[i] = i;
    }

    std::shuffle(table.begin(), table.end(), std::mt19937(key));

    return table;
}

std::vector<uint8_t> Cryptographer::BuildDecryptTable(int key)
{
    std::vector<uint8_t> cryptTable = BuildCryptTable(key);
    std::vector<uint8_t> result(256);

    for (int i = 0; i < 256; i++)
    {
        result[cryptTable[i]] = i;
    }

    return result;
}
