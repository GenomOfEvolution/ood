#pragma once
#include <catch.hpp>
#include <fakeit.hpp>

#include "../ShapesApp/Models/Document/DocumentModel.h"
#include "../ShapesApp/Models/Selection/DocumentSelection.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Ellipse/CEllipse.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Rectangle/CRectangle.h"

using namespace fakeit;
using namespace std::filesystem;

TEST_CASE("DocumentSelection methods")
{
    Mock<IHistory> historyMock;
    Mock<ISerializer> serializerMock;
    Rect fieldArea(0, 0, 800, 600);

    auto history = std::shared_ptr<IHistory>(&historyMock.get(), [](auto*) {});
    auto saver = std::shared_ptr<ISerializer>(&serializerMock.get(), [](auto*) {});

    When(Method(historyMock, CanUndo)).Return(false);
    When(Method(historyMock, CanRedo)).Return(false);
    Fake(Method(historyMock, Undo), Method(historyMock, Redo));
    Fake(Method(serializerMock, Serialize));
    Fake(Method(serializerMock, Deserialize));
    Fake(Method(serializerMock, SetDocument));

    GIVEN("A selection without document")
    {
        DocumentSelection selection;

        WHEN("Getting selected indexes")
        {
            auto indexes = selection.GetSelectedIndexes();

            THEN("Returns empty vector")
            {
                REQUIRE(indexes.empty());
            }
        }
    }

    GIVEN("A document with two non-overlapping shapes")
    {
        auto doc = std::make_shared<DocumentModel>(std::move(history), std::move(saver), fieldArea);

        // Create rectangle at (100,100) with size 50x50
        auto rect = std::make_shared<CRectangle>(Point(100, 100), 50, 50);
        doc->AddItem(std::make_unique<DocumentItem>(rect));

        // Create ellipse at center (200,200) with radii 30x40
        auto ellipse = std::make_shared<CEllipse>(Point(200, 200), 30, 40);
        doc->AddItem(std::make_unique<DocumentItem>(ellipse));

        DocumentSelection selection;
        selection.SetDocument(doc);

        WHEN("Selecting a point inside rectangle")
        {
            // Point (110,110) is inside the rectangle
            selection.SelectItem(Point{ 110, 110 });

            THEN("Only rectangle is selected")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 1);
                REQUIRE(indexes[0] == 0);
            }
        }

        WHEN("Selecting a point inside ellipse")
        {
            // Point (200,200) is the center of the ellipse
            selection.SelectItem(Point{ 200, 200 });

            THEN("Only ellipse is selected")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 1);
                REQUIRE(indexes[0] == 1);
            }
        }

        WHEN("Selecting a point outside all shapes")
        {
            selection.SelectItem(Point{ 50, 50 });

            THEN("No items are selected")
            {
                REQUIRE(selection.GetSelectedIndexes().empty());
            }
        }

        WHEN("Using additive selection")
        {
            // First select rectangle
            selection.SelectItem(Point{ 110, 110 });
            REQUIRE(selection.GetSelectedIndexes().size() == 1);

            // Then add ellipse in additive mode
            selection.SelectItem(Point{ 200, 200 }, true);

            THEN("Both items are selected")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 2);
                REQUIRE(std::find(indexes.begin(), indexes.end(), 0) != indexes.end());
                REQUIRE(std::find(indexes.begin(), indexes.end(), 1) != indexes.end());
            }
        }

        WHEN("Toggling selection in additive mode")
        {
            // Select both items
            selection.SelectItem(Point{ 110, 110 }, true);
            selection.SelectItem(Point{ 200, 200 }, true);
            REQUIRE(selection.GetSelectedIndexes().size() == 2);

            // Toggle off the ellipse
            selection.SelectItem(Point{ 200, 200 }, true);

            THEN("Ellipse is removed from selection")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 1);
                REQUIRE(indexes[0] == 0);
            }
        }
    }

    GIVEN("A document with overlapping shapes")
    {
        auto doc = std::make_shared<DocumentModel>(
            std::move(history),
            std::move(saver),
            fieldArea
        );

        // Bottom rectangle at (100,100) with size 200x200
        auto bottomRect = std::make_shared<CRectangle>(Point(100, 100), 200, 200);
        doc->AddItem(std::make_unique<DocumentItem>(bottomRect));

        // Top ellipse centered at (150,150) with radii 50x50 (will overlap bottom rectangle)
        auto topEllipse = std::make_shared<CEllipse>(Point(150, 150), 50, 50);
        doc->AddItem(std::make_unique<DocumentItem>(topEllipse));

        DocumentSelection selection;
        selection.SetDocument(doc);

        WHEN("Selecting point in overlapping area")
        {
            // Point (150,150) is inside both shapes
            selection.SelectItem(Point{ 150, 150 });

            THEN("Top ellipse is selected (last added item)")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 1);
                REQUIRE(indexes[0] == 1); // Ellipse has index 1 (added last)
            }
        }

        WHEN("Selecting point only in bottom rectangle")
        {
            // Point (250,150) is outside ellipse but inside rectangle
            selection.SelectItem(Point{ 250, 150 });

            THEN("Bottom rectangle is selected")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 1);
                REQUIRE(indexes[0] == 0);
            }
        }
    }

    GIVEN("A document with multiple shapes and selection operations")
    {
        auto doc = std::make_shared<DocumentModel>(
            std::move(history),
            std::move(saver),
            fieldArea
        );

        // Add three shapes
        doc->AddItem(std::make_unique<DocumentItem>(std::make_shared<CRectangle>(Point(50, 50), 40, 40)));
        doc->AddItem(std::make_unique<DocumentItem>(std::make_shared<CRectangle>(Point(100, 100), 60, 60)));
        doc->AddItem(std::make_unique<DocumentItem>(std::make_shared<CEllipse>(Point(200, 200), 80, 80)));

        DocumentSelection selection;
        selection.SetDocument(doc);

        WHEN("Directly adding indexes")
        {
            selection.AddIndex(0);
            selection.AddIndex(2);

            THEN("Specified indexes are selected")
            {
                auto indexes = selection.GetSelectedIndexes();
                REQUIRE(indexes.size() == 2);
                REQUIRE(indexes[0] == 0);
                REQUIRE(indexes[1] == 2);
            }
        }

        WHEN("Clearing selection after multiple selections")
        {
            selection.SelectItem(Point{ 60, 60 }, true);  // First rectangle
            selection.SelectItem(Point{ 110, 110 }, true); // Second rectangle
            REQUIRE(selection.GetSelectedIndexes().size() == 2);

            selection.ClearSelection();

            THEN("Selection is empty")
            {
                REQUIRE(selection.GetSelectedIndexes().empty());
            }
        }
    }
}
