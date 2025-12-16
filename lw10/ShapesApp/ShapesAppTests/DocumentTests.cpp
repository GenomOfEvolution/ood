#pragma once
#include <catch.hpp>
#include <fakeit.hpp>

#include "../ShapesApp/Models/Document/DocumentModel.h"

using namespace fakeit;
using namespace std::filesystem;

TEST_CASE("DocumentModel methods")
{
    Mock<IHistory> historyMock;
    Mock<ISerializer> serializerMock;
    Rect fieldArea(0, 0, 800, 600);

    auto history = std::shared_ptr<IHistory>(&historyMock.get(), [](auto*) {});
    auto saver = std::shared_ptr<ISerializer>(&serializerMock.get(), [](auto*) {});

    DocumentModel model(std::move(history), std::move(saver), fieldArea);

    When(Method(historyMock, CanUndo)).Return(false);
    When(Method(historyMock, CanRedo)).Return(false);
    Fake(Method(historyMock, Undo), Method(historyMock, Redo));
    Fake(Method(serializerMock, Serialize));
    Fake(Method(serializerMock, Deserialize));
    Fake(Method(serializerMock, SetDocument));

    GIVEN("A document with no save path set")
    {
        WHEN("Save is called")
        {
            THEN("Throws exception because no save path is set")
            {
                REQUIRE_THROWS_AS(model.Save(), std::runtime_error);
            }
        }
    }

    GIVEN("A document with save path set to \"document.doc\"")
    {
        model.SaveAs("document.doc");

        WHEN("Save is called")
        {
            model.Save();
            THEN("Serializer is called with the correct path")
            {
                Verify(Method(serializerMock, Serialize)).Twice();
                Verify(Method(serializerMock, Serialize).Using("document.doc"));
            }
        }
    }

    GIVEN("SaveAs is called with empty path")
    {
        WHEN("SaveAs is executed with empty path")
        {
            THEN("Throws invalid_argument exception")
            {
                REQUIRE_THROWS_AS(model.SaveAs(""), std::invalid_argument);
            }
        }
    }

    GIVEN("Load is called with empty path")
    {
        WHEN("Load is executed with empty path")
        {
            THEN("Throws invalid_argument exception")
            {
                REQUIRE_THROWS_AS(model.Load(""), std::invalid_argument);
            }
        }
    }

    GIVEN("History can undo")
    {
        When(Method(historyMock, CanUndo)).Return(true);

        WHEN("CanUndo is called")
        {
            bool canUndo = model.CanUndo();
            THEN("Returns true")
            {
                REQUIRE(canUndo == true);
                Verify(Method(historyMock, CanUndo)).Once();
            }
        }

        WHEN("Undo is called")
        {
            model.Undo();
            THEN("History Undo is executed")
            {
                Verify(Method(historyMock, Undo)).Once();
            }
        }
    }

    GIVEN("History cannot undo")
    {
        When(Method(historyMock, CanUndo)).Return(false);

        WHEN("Undo is called")
        {
            model.Undo();
            THEN("History Undo is not executed")
            {
                Verify(Method(historyMock, Undo)).Never();
            }
        }
    }

    GIVEN("History can redo")
    {
        When(Method(historyMock, CanRedo)).Return(true);

        WHEN("CanRedo is called")
        {
            bool canRedo = model.CanRedo();
            THEN("Returns true")
            {
                REQUIRE(canRedo == true);
                Verify(Method(historyMock, CanRedo)).Once();
            }
        }

        WHEN("Redo is called")
        {
            model.Redo();
            THEN("History Redo is executed")
            {
                Verify(Method(historyMock, Redo)).Once();
            }
        }
    }

    GIVEN("History cannot redo")
    {
        When(Method(historyMock, CanRedo)).Return(false);

        WHEN("Redo is called")
        {
            model.Redo();
            THEN("History Redo is not executed")
            {
                Verify(Method(historyMock, Redo)).Never();
            }
        }
    }
}