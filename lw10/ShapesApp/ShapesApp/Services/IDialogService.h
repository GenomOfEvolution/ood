#pragma once
#include <string>
#include <optional>

class IDialogService
{
public:
    virtual ~IDialogService() = default;
    virtual std::optional<std::string> GetSaveFilePath() = 0;
    virtual std::optional<std::string> GetOpenFilePath() = 0;
};