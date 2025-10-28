#pragma once
#include <catch.hpp>
#include <fakeit.hpp>

#include "../Editor/Command/ICommand.h"
#include "../Editor/Command/MergableCommand/IMergableCommand.h"
#include "../Editor/History/History.h"

using namespace fakeit;

class MockCommand : public ICommand
{
public:
    void Execute() override { executeCount++; }
    void Unexecute() override { unexecuteCount++; }
    bool ShouldSaveToHistory() const override { return true; }

    int executeCount = 0;
    int unexecuteCount = 0;
};

class MockMergableCommand : public IMergableCommand 
{
public:
    void DoExecute() override { executeCount++; }
    void DoUnexecute() override { unexecuteCount++; }

    bool CanMergeWith(const ICommand& other) const override 
    {
        auto otherCmd = dynamic_cast<const MockMergableCommand*>(&other);
        return otherCmd && canMerge;
    }

    void MergeWith(std::unique_ptr<ICommand> other) override 
    {
        mergeCount++;
    }

    int executeCount = 0;
    int unexecuteCount = 0;
    int mergeCount = 0;
    bool canMerge = false;
};

TEST_CASE("History - AddAndExecuteCommand")
{
    History history;

    SECTION("Should execute command and add to history") 
    {
        auto command = std::make_unique<MockCommand>();
        auto& commandRef = *command;

        history.AddAndExecuteCommand(std::move(command));

        REQUIRE(commandRef.executeCount == 1);
        REQUIRE(history.CanUndo() == true);
        REQUIRE(history.CanRedo() == false);
    }

    SECTION("Should remove redo branch when adding new command after undo") 
    {
        auto command1 = std::make_unique<MockCommand>();
        auto& cmd1Ref = *command1;
        history.AddAndExecuteCommand(std::move(command1));

        auto command2 = std::make_unique<MockCommand>();
        auto& cmd2Ref = *command2;
        history.AddAndExecuteCommand(std::move(command2));

        history.Undo();
        REQUIRE(history.CanRedo() == true);

        auto command3 = std::make_unique<MockCommand>();
        history.AddAndExecuteCommand(std::move(command3));

        REQUIRE(history.CanRedo() == false);
    }
}

TEST_CASE("History - Undo/Redo") {
    History history;

    SECTION("CanUndo should return correct values") {
        REQUIRE(history.CanUndo() == false);

        auto command = std::make_unique<MockCommand>();
        history.AddAndExecuteCommand(std::move(command));

        REQUIRE(history.CanUndo() == true);

        history.Undo();
        REQUIRE(history.CanUndo() == false);
    }

    SECTION("CanRedo should return correct values") {
        REQUIRE(history.CanRedo() == false);

        auto command = std::make_unique<MockCommand>();
        history.AddAndExecuteCommand(std::move(command));

        REQUIRE(history.CanRedo() == false);

        history.Undo();
        REQUIRE(history.CanRedo() == true);

        history.Redo();
        REQUIRE(history.CanRedo() == false);
    }

    SECTION("Undo should unexecute command") {
        auto command = std::make_unique<MockCommand>();
        auto& commandRef = *command;

        history.AddAndExecuteCommand(std::move(command));
        history.Undo();

        REQUIRE(commandRef.unexecuteCount == 1);
    }

    SECTION("Redo should execute command again") 
    {
        auto command = std::make_unique<MockCommand>();
        auto& commandRef = *command;

        history.AddAndExecuteCommand(std::move(command));
        history.Undo();
        history.Redo();

        REQUIRE(commandRef.executeCount == 2);
        REQUIRE(commandRef.unexecuteCount == 1);
    }

    SECTION("Multiple undo/redo operations") 
    {
        auto command1 = std::make_unique<MockCommand>();
        auto& cmd1Ref = *command1;
        auto command2 = std::make_unique<MockCommand>();
        auto& cmd2Ref = *command2;

        history.AddAndExecuteCommand(std::move(command1));
        history.AddAndExecuteCommand(std::move(command2));

        history.Undo();
        REQUIRE(cmd2Ref.unexecuteCount == 1);

        history.Undo();
        REQUIRE(cmd1Ref.unexecuteCount == 1);

        history.Redo();
        REQUIRE(cmd1Ref.executeCount == 2); 

        history.Redo();
        REQUIRE(cmd2Ref.executeCount == 2);
    }
}

TEST_CASE("History - Size limit") 
{
    History history;
    const int MAX_HISTORY_SIZE = 10;
    SECTION("Should respect maximum history size") 
    {
        for (int i = 0; i < 15; ++i) 
        {
            auto command = std::make_unique<MockCommand>();
            history.AddAndExecuteCommand(std::move(command));
        }

        for (size_t i = 0; i < MAX_HISTORY_SIZE; ++i) 
        {
            REQUIRE(history.CanUndo() == true);
            history.Undo();
        }

        REQUIRE(history.CanUndo() == false);
    }

    SECTION("Should remove oldest command when exceeding size limit") 
    {
        std::vector<MockCommand*> commands;

        for (int i = 0; i <= MAX_HISTORY_SIZE; ++i) 
        {
            auto command = std::make_unique<MockCommand>();
            commands.push_back(command.get());
            history.AddAndExecuteCommand(std::move(command));
        }

        for (size_t i = 0; i < MAX_HISTORY_SIZE; ++i)
        {
            history.Undo();
        }

        REQUIRE(history.CanUndo() == false);
    }
}

TEST_CASE("History - Command merging")
{
    History history;

    SECTION("Should merge commands when possible") 
    {
        auto command1 = std::make_unique<MockMergableCommand>();
        auto& cmd1Ref = *command1;
        command1->canMerge = true;

        auto command2 = std::make_unique<MockMergableCommand>();
        auto& cmd2Ref = *command2;
        command2->canMerge = true;

        history.AddAndExecuteCommand(std::move(command1));
        history.AddAndExecuteCommand(std::move(command2));

        REQUIRE(cmd1Ref.mergeCount == 1);
    }

    SECTION("Should not merge when CanMergeWith returns false")
    {
        auto command1 = std::make_unique<MockMergableCommand>();
        auto& cmd1Ref = *command1;
        command1->canMerge = false;

        auto command2 = std::make_unique<MockMergableCommand>();
        auto& cmd2Ref = *command2;
        command2->canMerge = false;

        history.AddAndExecuteCommand(std::move(command1));
        history.AddAndExecuteCommand(std::move(command2));

        REQUIRE(cmd1Ref.mergeCount == 0);
        REQUIRE(history.CanUndo() == true);
    }

    SECTION("Should not merge with empty history")
    {
        auto command = std::make_unique<MockMergableCommand>();
        auto& cmdRef = *command;
        cmdRef.canMerge = true;

        history.AddAndExecuteCommand(std::move(command));

        REQUIRE(cmdRef.mergeCount == 0);
        REQUIRE(history.CanUndo() == true);
    }

    SECTION("Should not merge different command types") 
    {
        auto command1 = std::make_unique<MockCommand>(); // Не IMergableCommand
        auto& cmd1Ref = *command1;

        auto command2 = std::make_unique<MockMergableCommand>();
        auto& cmd2Ref = *command2;
        cmd2Ref.canMerge = true;

        history.AddAndExecuteCommand(std::move(command1));
        history.AddAndExecuteCommand(std::move(command2));

        REQUIRE(cmd2Ref.mergeCount == 0);
        REQUIRE(history.CanUndo() == true);
    }
}

TEST_CASE("History - Complex scenarios") 
{
    History history;

    SECTION("Add command after multiple undo operations") 
    {
        auto cmd1 = std::make_unique<MockCommand>();
        auto& cmd1Ref = *cmd1;
        auto cmd2 = std::make_unique<MockCommand>();
        auto& cmd2Ref = *cmd2;
        auto cmd3 = std::make_unique<MockCommand>();
        auto& cmd3Ref = *cmd3;

        history.AddAndExecuteCommand(std::move(cmd1));
        history.AddAndExecuteCommand(std::move(cmd2));
        history.AddAndExecuteCommand(std::move(cmd3));

        history.Undo();
        history.Undo();

        REQUIRE(history.CanRedo() == true);

        auto cmd4 = std::make_unique<MockCommand>();
        auto& cmd4Ref = *cmd4;
        history.AddAndExecuteCommand(std::move(cmd4));

        REQUIRE(history.CanRedo() == false);
        REQUIRE(history.CanUndo() == true);

        history.Undo();
        REQUIRE(history.CanUndo() == true);
        history.Undo();
        REQUIRE(history.CanUndo() == false);
    }

    SECTION("Mixed mergable and non-mergable commands") 
    {
        auto mergable1 = std::make_unique<MockMergableCommand>();
        auto& m1Ref = *mergable1;
        m1Ref.canMerge = true;

        auto mergable2 = std::make_unique<MockMergableCommand>();
        auto& m2Ref = *mergable2;
        m2Ref.canMerge = true;

        auto nonMergable = std::make_unique<MockCommand>();
        auto& nmRef = *nonMergable;

        history.AddAndExecuteCommand(std::move(mergable1));
        REQUIRE(m1Ref.executeCount == 1);

        history.AddAndExecuteCommand(std::move(mergable2));
        REQUIRE(m1Ref.mergeCount == 1);

        history.AddAndExecuteCommand(std::move(nonMergable));
        REQUIRE(nmRef.executeCount == 1);

        history.Undo();
        REQUIRE(nmRef.unexecuteCount == 1);

        history.Undo();
        REQUIRE(m1Ref.unexecuteCount == 1);

        REQUIRE(history.CanUndo() == false);
    }
}