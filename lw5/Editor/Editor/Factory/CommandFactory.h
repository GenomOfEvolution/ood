#pragma once
#include "ICommandFactory.h"
#include <map>
#include <functional>

class CommandFactory : public ICommandFactory
{
public:
    std::unique_ptr<ICommand> CreateCommand(
        IDocument& doc,
        ISaver& saver,
        const std::string& description) override;

private:
    static std::unique_ptr<ICommand> CreateInsertImageCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateInsertParagraphCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateReplaceTextCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateResizeImageCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateSetTitleCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateDeleteItemCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateListCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateSaveCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateUndoCommand(IDocument& doc, ISaver& saver, std::istream& input);
    static std::unique_ptr<ICommand> CreateRedoCommand(IDocument& doc, ISaver& saver, std::istream& input);

    using CommandCreator = std::function<std::unique_ptr<ICommand>(IDocument& doc, ISaver& saver, std::istream& input)>;
    using CommandCreators = std::map<std::string, CommandCreator>;

    static inline const CommandCreators m_actionMap = {
        { "InsertImage", CreateInsertImageCommand },
        { "InsertParagraph", CreateInsertParagraphCommand },
        { "ReplaceText", CreateReplaceTextCommand },
        { "ResizeImage", CreateResizeImageCommand },
        { "SetTitle", CreateSetTitleCommand },
        { "DeleteItem", CreateDeleteItemCommand },
        { "List", CreateListCommand },
        { "Save", CreateSaveCommand },
        { "Undo", CreateUndoCommand },
        { "Redo", CreateRedoCommand },
    };
};