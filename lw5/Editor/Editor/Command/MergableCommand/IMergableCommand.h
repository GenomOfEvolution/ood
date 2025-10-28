#pragma once
#include "../AbstractUndoableCommand.h"
#include <string>
#include <memory>

class IMergableCommand : public AbstractUndoableCommand
{
public:
    virtual bool CanMergeWith(const ICommand& other) const = 0;
    virtual void MergeWith(std::unique_ptr<ICommand> other) = 0;
};