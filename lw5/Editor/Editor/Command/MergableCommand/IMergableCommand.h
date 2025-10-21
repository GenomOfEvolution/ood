#pragma once
#include "../AbstractCommand.h"
#include <string>
#include <memory>

class IMergableCommand : public AbstractCommand
{
public:
    virtual bool CanMergeWith(const ICommand& other) const = 0;
    virtual void MergeWith(std::unique_ptr<ICommand> other) = 0;
};