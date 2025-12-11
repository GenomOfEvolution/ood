#include "AddShapeCommand.h"
#include "../../Factory/DocItemFactory/DocItemFactory.h"

AddShapeCommand::AddShapeCommand(
    IDocument& doc,
    ISelection& selection,
    const std::string& description,
    ShapeAddedCallback onShapeAdded,
    ShapeRemovedCallback onShapeRemoved
)
    : m_document(doc)
    , m_selection(selection)
    , m_shapeDescription(description)
    , m_onShapeRemoved(std::move(onShapeRemoved))
    , m_onShapeAdded(std::move(onShapeAdded))
{
}

void AddShapeCommand::DoExecute()
{
    m_insertPos = m_document.GetItemsCount();

    m_document.AddItem(DocItemFactory::CreateItem(m_shapeDescription));

    m_selection.ClearSelection();
    m_selection.AddIndex(m_insertPos);

    if (m_onShapeAdded)
    {
        m_onShapeAdded();
    }
}

void AddShapeCommand::DoUnexecute()
{
    m_document.RemoveItemAtIndex(m_insertPos);

    m_selection.ClearSelection();

    if (m_onShapeRemoved) 
    {
        m_onShapeRemoved();
    }
}