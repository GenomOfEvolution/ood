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
#include "../Command/ExitCommand/ExitCommand.h"
#include "../Command/HelpCommand/HelpCommand.h"
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

CommandFactory::CommandFactory(IDocument& doc, ISaver& saver, Menu& menu)
    : m_document(doc)
    , m_saver(saver)
    , m_menu(menu)
    , m_actionMap {
          { "InsertImage", [this](std::istream& input) { return CreateInsertImageCommand(input); }},
          { "InsertParagraph", [this](std::istream& input) { return CreateInsertParagraphCommand(input); }},
          { "ReplaceText", [this](std::istream& input) { return CreateReplaceTextCommand(input); }},
          { "ResizeImage", [this](std::istream& input) { return CreateResizeImageCommand(input); }},
          { "SetTitle", [this](std::istream& input) { return CreateSetTitleCommand(input); }},
          { "DeleteItem", [this](std::istream& input) { return CreateDeleteItemCommand(input); }},
          { "List", [this](std::istream& input) { return CreateListCommand(input); }},
          { "Save", [this](std::istream& input) { return CreateSaveCommand(input); }},
          { "Undo", [this](std::istream& input) { return CreateUndoCommand(input); }},
          { "Redo", [this](std::istream& input) { return CreateRedoCommand(input); }},
          { "Help", [this](std::istream& input) { return CreateHelpCommand(input); }},
          { "Exit", [this](std::istream& input) { return CreateExitCommand(input); }}
    }
{
}

std::unique_ptr<ICommand> CommandFactory::CreateCommand(const std::string& description)
{
    std::istringstream input(description);

    std::string commandType;
    input >> commandType;

    auto it = m_actionMap.find(commandType);
    if (it == m_actionMap.end())
    {
        throw std::runtime_error("Unknown command!");
    }

    return it->second(input);
}

std::unique_ptr<ICommand> CommandFactory::CreateInsertImageCommand(std::istream& input)
{
    auto index = ReadIndex(input);
    int width, height;
    std::string pathStr;
    input >> width >> height >> pathStr;

    return std::make_unique<InsertImageCommand>(m_document, m_saver, index, width, height, std::filesystem::path(pathStr));
}

std::unique_ptr<ICommand> CommandFactory::CreateInsertParagraphCommand(std::istream& input)
{
    auto index = ReadIndex(input);
    std::string text = ReadRemainingText(input);

    return std::make_unique<InsertParagraphCommand>(m_document, index, text);
}

std::unique_ptr<ICommand> CommandFactory::CreateReplaceTextCommand(std::istream& input)
{
    size_t index;
    input >> index;
    std::string newText = ReadRemainingText(input);

    auto item = m_document.GetItem(index);
    if (!item.GetParagraph())
    {
        throw std::runtime_error("Item at index " + std::to_string(index) + " is not a paragraph");
    }

    return std::make_unique<ReplaceTextCommand>(item.GetParagraph(), newText);
}

std::unique_ptr<ICommand> CommandFactory::CreateResizeImageCommand(std::istream& input)
{
    size_t index;
    int width, height;
    input >> index >> width >> height;

    auto item = m_document.GetItem(index);
    if (item.GetImage() == nullptr)
    {
        throw std::runtime_error("Item at index " + std::to_string(index) + " is not a image");
    }

    return std::make_unique<ResizeImageCommand>(item.GetImage(), width, height);
}

std::unique_ptr<ICommand> CommandFactory::CreateSetTitleCommand(std::istream& input)
{
    std::string newTitle = ReadRemainingText(input);

    return std::make_unique<SetTitleCommand>(m_document, newTitle);
}

std::unique_ptr<ICommand> CommandFactory::CreateDeleteItemCommand(std::istream& input)
{
    size_t index;
    input >> index;

    return std::make_unique<DeleteItemCommand>(m_document, m_saver, index);
}

std::unique_ptr<ICommand> CommandFactory::CreateListCommand(std::istream& input)
{
    return std::make_unique<ListCommand>(m_document);
}

std::unique_ptr<ICommand> CommandFactory::CreateSaveCommand(std::istream& input)
{
    std::string pathStr;
    input >> pathStr;

    return std::make_unique<SaveCommand>(m_document, std::filesystem::path(pathStr));
}

std::unique_ptr<ICommand> CommandFactory::CreateUndoCommand(std::istream& input)
{
    return std::make_unique<UndoCommand>(m_document);
}

std::unique_ptr<ICommand> CommandFactory::CreateRedoCommand( std::istream& input)
{
    return std::make_unique<RedoCommand>(m_document);
}

std::unique_ptr<ICommand> CommandFactory::CreateHelpCommand(std::istream& input)
{
    return std::make_unique<HelpCommand>(m_menu);
}

std::unique_ptr<ICommand> CommandFactory::CreateExitCommand(std::istream& input)
{
    return std::make_unique<ExitCommand>(m_menu);
}
