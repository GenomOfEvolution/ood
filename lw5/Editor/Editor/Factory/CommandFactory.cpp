#include "CommandFactory.h"
#include "../Command/DeleteItemCommand/DeleteItemCommand.h"
#include "../Command/InsertImageCommand/InsertImageCommand.h"
#include "../Command/InsertParagraphCommand/InsertParagraphCommand.h"
#include "../Command/ListCommand/ListCommand.h"
#include "../Command/MergableCommand/ReplaceTextCommand/ReplaceTextCommand.h"
#include "../Command/MergableCommand/ResizeImageCommand/ResizeImageCommand.h"
#include "../Command/MergableCommand/SetTitleCommand/SetTitleCommand.h"
#include "../Command/RedoCommand/RedoCommand.h"
#include "../Command/SaveCommand/SaveCommand.h"
#include "../Command/UndoCommand/UndoCommand.h"
#include <sstream>
#include <filesystem>

namespace
{
    std::optional<size_t> ReadIndex(std::istream& input)
    {
        std::string indexStr;
        input >> indexStr;

        if (indexStr == "end")
        {
            return std::nullopt;
        }

        size_t index;
        std::istringstream indexStream(indexStr);
        if (indexStream >> index)
        {
            return index;
        }

        throw std::runtime_error("Invalid index: " + indexStr);
    }

    std::string ReadRemainingText(std::istream& input)
    {
        std::string text;
        std::getline(input, text);

        text.erase(0, text.find_first_not_of(" \t"));

        return text;
    }
} // namespace

std::unique_ptr<ICommand> CommandFactory::CreateCommand(IDocument& doc, ISaver& saver, const std::string& description)
{
    std::istringstream input(description);

    std::string commandType;
    input >> commandType;

    auto it = m_actionMap.find(commandType);
    if (it == m_actionMap.end())
    {
        throw std::runtime_error("Unknown command!");
    }

    return it->second(doc, saver, input);
}

std::unique_ptr<ICommand> CommandFactory::CreateInsertImageCommand(IDocument& doc, ISaver& saver, std::istream& input)
{
    auto index = ReadIndex(input);
    int width, height;
    std::string pathStr;
    input >> width >> height >> pathStr;

    return std::make_unique<InsertImageCommand>(doc, saver, index, width, height, std::filesystem::path(pathStr));
}

std::unique_ptr<ICommand> CommandFactory::CreateInsertParagraphCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    auto index = ReadIndex(input);
    std::string text = ReadRemainingText(input);

    return std::make_unique<InsertParagraphCommand>(doc, index, text);
}

std::unique_ptr<ICommand> CommandFactory::CreateReplaceTextCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    size_t index;
    input >> index;
    std::string newText = ReadRemainingText(input);

    auto item = doc.GetItem(index);
    if (!item.GetParagraph())
    {
        throw std::runtime_error("Item at index " + std::to_string(index) + " is not a paragraph");
    }

    return std::make_unique<ReplaceTextCommand>(item.GetParagraph(), newText);
}

std::unique_ptr<ICommand> CommandFactory::CreateResizeImageCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    size_t index;
    int width, height;
    input >> index >> width >> height;

    auto item = doc.GetItem(index);
    if (item.GetImage() == nullptr)
    {
        throw std::runtime_error("Item at index " + std::to_string(index) + " is not a image");
    }

    return std::make_unique<ResizeImageCommand>(item.GetImage(), width, height);
}

std::unique_ptr<ICommand> CommandFactory::CreateSetTitleCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    std::string newTitle = ReadRemainingText(input);
    std::string currentTitle = doc.GetTitle();

    return std::make_unique<SetTitleCommand>(currentTitle, newTitle);
}

std::unique_ptr<ICommand> CommandFactory::CreateDeleteItemCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    size_t index;
    input >> index;

    return std::make_unique<DeleteItemCommand>(doc, saver, index);
}

std::unique_ptr<ICommand> CommandFactory::CreateListCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    return std::make_unique<ListCommand>(doc);
}

std::unique_ptr<ICommand> CommandFactory::CreateSaveCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    std::string pathStr;
    input >> pathStr;

    return std::make_unique<SaveCommand>(doc, std::filesystem::path(pathStr));
}

std::unique_ptr<ICommand> CommandFactory::CreateUndoCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    return std::make_unique<UndoCommand>(doc);
}

std::unique_ptr<ICommand> CommandFactory::CreateRedoCommand(IDocument& doc, ISaver&saver, std::istream& input)
{
    return std::make_unique<RedoCommand>(doc);
}