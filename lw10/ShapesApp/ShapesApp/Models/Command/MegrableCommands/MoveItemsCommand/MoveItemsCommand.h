#pragma once
#include "../IMergableCommand.h"
#include "../../../Document/IDocument.h"
#include "../../../Selection/ISelection.h"
#include <vector>

using ItemsMovedCallback = std::function<void(const std::vector<size_t>& indexes, double dx, double dy)>;

class MoveItemsCommand : public IMergableCommand
{
public:
    MoveItemsCommand(
        IDocument& doc,
        ISelection& selection,
        Point delta,
        ItemsMovedCallback onItemsMoved = nullptr
    );

    bool CanMergeWith(const ICommand& other) const override;
    void MergeWith(std::unique_ptr<ICommand> other) override;

private:
    void DoExecute() override;
    void DoUnexecute() override;

    IDocument& m_document;
    ISelection& m_selection;
    Point m_delta;
    ItemsMovedCallback m_onItemsMoved;

    std::vector<size_t> m_selectedIndexes;
    std::vector<Rect> m_originalRects;
    Point m_actualDelta;
};