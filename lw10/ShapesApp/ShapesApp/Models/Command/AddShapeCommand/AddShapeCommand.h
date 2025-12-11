#pragma once
#include "../../Document/IDocument.h"
#include "../../Selection/ISelection.h"
#include "../AbstractUndoableCommand.h"
#include <functional>

using ShapeAddedCallback = std::function<void()>;
using ShapeRemovedCallback = std::function<void()>;

class AddShapeCommand : public AbstractUndoableCommand
{
public:
    AddShapeCommand(
        IDocument& doc,
        ISelection& selection,
        const std::string& description,
        ShapeAddedCallback onShapeAdded = nullptr,
        ShapeRemovedCallback onShapeRemoved = nullptr
    );

private:
    void DoExecute() override;
    void DoUnexecute() override;

    IDocument& m_document;
    ISelection& m_selection;
    std::string m_shapeDescription;
    size_t m_insertPos = 0;

    ShapeAddedCallback m_onShapeAdded;
    ShapeRemovedCallback m_onShapeRemoved;
};