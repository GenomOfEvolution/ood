#pragma once
#include "ICommandFactory.h"
#include "../Document/IDocument.h"
#include "../Saver/ISaver.h"
#include "../Menu/Menu.h"

#include <map>
#include <functional>

class CommandFactory : public ICommandFactory
{
public:
    CommandFactory(IDocument& doc, ISaver& saver, Menu& menu);
    std::unique_ptr<ICommand> CreateCommand(const std::string& description) override;

private:
    std::unique_ptr<ICommand> CreateInsertImageCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateInsertParagraphCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateReplaceTextCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateResizeImageCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateSetTitleCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateDeleteItemCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateListCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateSaveCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateUndoCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateRedoCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateHelpCommand(std::istream& input);
    std::unique_ptr<ICommand> CreateExitCommand(std::istream& input);

    using CommandCreator = std::function<std::unique_ptr<ICommand>(std::istream&)>;
    using CommandCreators = std::map<std::string, CommandCreator>;

    const CommandCreators m_actionMap;

    IDocument& m_document;
    ISaver& m_saver;
    Menu& m_menu;
};