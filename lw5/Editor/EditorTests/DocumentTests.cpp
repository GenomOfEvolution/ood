#pragma once
#include <catch.hpp>
#include <fakeit.hpp>
#include "../Editor/Document/HtmlDocument/HtmlDocument.h"
#include "../Editor/History/IHistory.h"
#include "../Editor/History/ICommandExecutor.h"
#include "../Editor/Saver/ISaver.h"
#include "../Editor/DocumentItem/DocumentItem.h"

using namespace fakeit;

class MockParagraph : public IParagraph 
{
public:
    MockParagraph(const std::string& text) : m_text(text) {}

    std::string GetText() const override { return m_text; }
    void SetText(const std::string& text) override {
        m_text = text;
        setTextCount++;
    }

    std::string m_text;
    int setTextCount = 0;
};

class MockImage : public IImage 
{
public:
    MockImage(const std::filesystem::path& path, int width, int height)
        : m_path(path), m_width(width), m_height(height) 
    {
    }

    std::filesystem::path GetPath() const override { return m_path; }
    void SetPath(const std::filesystem::path& path) override { m_path = path; }

    int GetWidth() const override { return m_width; }
    int GetHeight() const override { return m_height; }
    void Resize(int width, int height) override {
        m_width = width;
        m_height = height;
        resizeCount++;
    }

    std::filesystem::path m_path;
    int m_width, m_height;
    int resizeCount = 0;
};

class MockHistory : public IHistory, public ICommandExecutor
{
public:
    void AddAndExecuteCommand(std::unique_ptr<ICommand>&& command) override 
    {
        addAndExecuteCount++;
        if (command) 
        {
            command->Execute();
        }
    }

    bool CanUndo() const override { return canUndo; }
    void Undo() override { undoCount++; }

    bool CanRedo() const override { return canRedo; }
    void Redo() override { redoCount++; }

    mutable bool canUndo = false;
    mutable bool canRedo = false;
    int addAndExecuteCount = 0;
    int undoCount = 0;
    int redoCount = 0;
};

class MockSaver : public ISaver 
{
public:
    void Save(const IDocument& document, const std::filesystem::path& path) override
    {
        saveCount++;
        lastPath = path;
    }

    std::filesystem::path SaveTempImage(const std::filesystem::path& srcPath) override 
    {
        return "";
    };

    void DeleteTempImage(const std::filesystem::path& srcPath) override {};

    int saveCount = 0;
    mutable std::filesystem::path lastPath;
};

TEST_CASE("HtmlDocument - Construction and basic properties")
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should initialize with empty title") 
    {
        REQUIRE(doc.GetTitle() == "");
    }

    SECTION("Should initialize with no items") 
    {
        REQUIRE(doc.GetItemsCount() == 0);
    }
}

TEST_CASE("HtmlDocument - Title operations") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should set and get title") 
    {
        doc.SetTitle("Test Title");
        REQUIRE(doc.GetTitle() == "Test Title");
    }

    SECTION("Should update title multiple times") 
    {
        doc.SetTitle("First Title");
        REQUIRE(doc.GetTitle() == "First Title");

        doc.SetTitle("Second Title");
        REQUIRE(doc.GetTitle() == "Second Title");
    }
}

TEST_CASE("HtmlDocument - InsertParagraph") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should insert paragraph at the end when no position specified") 
    {
        auto paragraph = doc.InsertParagraph("Test text");

        REQUIRE(paragraph != nullptr);
        REQUIRE(paragraph->GetText() == "Test text");
        REQUIRE(doc.GetItemsCount() == 1);

        auto item = doc.GetItem(0);
        REQUIRE(item.GetParagraph() != nullptr);
        REQUIRE(item.GetParagraph()->GetText() == "Test text");
    }

    SECTION("Should insert paragraph at specified position") 
    {
        // Добавляем первый параграф
        doc.InsertParagraph("First");

        // Вставляем второй параграф в начало
        auto paragraph = doc.InsertParagraph("Second", 0);

        REQUIRE(paragraph != nullptr);
        REQUIRE(paragraph->GetText() == "Second");
        REQUIRE(doc.GetItemsCount() == 2);

        // Проверяем порядок
        REQUIRE(doc.GetItem(0).GetParagraph()->GetText() == "Second");
        REQUIRE(doc.GetItem(1).GetParagraph()->GetText() == "First");
    }

    SECTION("Should insert paragraph at the end when position equals count") {
        doc.InsertParagraph("First");
        doc.InsertParagraph("Second", 1);

        REQUIRE(doc.GetItemsCount() == 2);
        REQUIRE(doc.GetItem(1).GetParagraph()->GetText() == "Second");
    }
}

TEST_CASE("HtmlDocument - InsertImage")
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should insert image at the end when no position specified") {
        auto image = doc.InsertImage("image.png", 100, 200);

        REQUIRE(image != nullptr);
        REQUIRE(image->GetPath() == "image.png");
        REQUIRE(image->GetWidth() == 100);
        REQUIRE(image->GetHeight() == 200);
        REQUIRE(doc.GetItemsCount() == 1);

        auto item = doc.GetItem(0);
        REQUIRE(item.GetImage() != nullptr);
        REQUIRE(item.GetImage()->GetPath() == "image.png");
    }

    SECTION("Should insert image at specified position") {
        doc.InsertParagraph("First");

        // Вставляем изображение в начало
        auto image = doc.InsertImage("test.jpg", 150, 250, 0);

        REQUIRE(image != nullptr);
        REQUIRE(doc.GetItemsCount() == 2);

        // Проверяем порядок
        REQUIRE(doc.GetItem(0).GetImage() != nullptr);
        REQUIRE(doc.GetItem(0).GetImage()->GetPath() == "test.jpg");
        REQUIRE(doc.GetItem(1).GetParagraph() != nullptr);
    }
}

TEST_CASE("HtmlDocument - Mixed content operations") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should handle mixed paragraphs and images") 
    {
        auto p1 = doc.InsertParagraph("First paragraph");
        auto img1 = doc.InsertImage("first.png", 100, 100);
        auto p2 = doc.InsertParagraph("Second paragraph");
        auto img2 = doc.InsertImage("second.png", 200, 200);

        REQUIRE(doc.GetItemsCount() == 4);

        REQUIRE(doc.GetItem(0).GetParagraph() != nullptr);
        REQUIRE(doc.GetItem(1).GetImage() != nullptr);
        REQUIRE(doc.GetItem(2).GetParagraph() != nullptr);
        REQUIRE(doc.GetItem(3).GetImage() != nullptr);

        REQUIRE(doc.GetItem(0).GetParagraph()->GetText() == "First paragraph");
        REQUIRE(doc.GetItem(1).GetImage()->GetPath() == "first.png");
        REQUIRE(doc.GetItem(2).GetParagraph()->GetText() == "Second paragraph");
        REQUIRE(doc.GetItem(3).GetImage()->GetPath() == "second.png");
    }
}

TEST_CASE("HtmlDocument - GetItem") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    doc.InsertParagraph("Test paragraph");
    doc.InsertImage("test.png", 100, 200);

    SECTION("Should get item with non-const version") 
    {
        auto item = doc.GetItem(0);
        REQUIRE(item.GetParagraph() != nullptr);

        auto item2 = doc.GetItem(1);
        REQUIRE(item2.GetImage() != nullptr);
    }

    SECTION("Should get item with const version") 
    {
        const HtmlDocument& constDoc = doc;

        auto item = constDoc.GetItem(0);
        REQUIRE(item.GetParagraph() != nullptr);

        auto item2 = constDoc.GetItem(1);
        REQUIRE(item2.GetImage() != nullptr);
    }
}

TEST_CASE("HtmlDocument - DeleteItem") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should delete item at valid position") 
    {
        doc.InsertParagraph("First");
        doc.InsertParagraph("Second");
        doc.InsertParagraph("Third");

        REQUIRE(doc.GetItemsCount() == 3);

        doc.DeleteItem(1);

        REQUIRE(doc.GetItemsCount() == 2);
        REQUIRE(doc.GetItem(0).GetParagraph()->GetText() == "First");
        REQUIRE(doc.GetItem(1).GetParagraph()->GetText() == "Third");
    }

    SECTION("Should delete single item") 
    {
        doc.InsertParagraph("Single item");
        REQUIRE(doc.GetItemsCount() == 1);

        doc.DeleteItem(0);
        REQUIRE(doc.GetItemsCount() == 0);
    }
}

TEST_CASE("HtmlDocument - Undo/Redo")
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("CanUndo/CanRedo should delegate to history") 
    {
        history->canUndo = true;
        history->canRedo = false;

        REQUIRE(doc.CanUndo() == true);
        REQUIRE(doc.CanRedo() == false);

        history->canUndo = false;
        history->canRedo = true;

        REQUIRE(doc.CanUndo() == false);
        REQUIRE(doc.CanRedo() == true);
    }

    SECTION("Undo/Redo should delegate to history") 
    {
        doc.Undo();
        REQUIRE(history->undoCount == 1);

        doc.Redo();
        REQUIRE(history->redoCount == 1);
    }
}

TEST_CASE("HtmlDocument - Save") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Should call saver with correct path") {
        doc.Save("test.html");

        REQUIRE(saver->saveCount == 1);
        REQUIRE(saver->lastPath == "test.html");
    }

    SECTION("Should save document with content") 
    {
        doc.SetTitle("Test Document");
        doc.InsertParagraph("First paragraph");
        doc.InsertImage("image.png", 100, 200);

        doc.Save("output.html");

        REQUIRE(saver->saveCount == 1);
        REQUIRE(saver->lastPath == "output.html");
    }
}

TEST_CASE("HtmlDocument - Complex scenarios") 
{
    auto history = std::make_shared<MockHistory>();
    auto saver = std::make_shared<MockSaver>();

    HtmlDocument doc(history, saver);

    SECTION("Insert, delete, and modify items") 
    {
        auto p1 = doc.InsertParagraph("Original text");
        auto img = doc.InsertImage("img.jpg", 100, 100);
        auto p2 = doc.InsertParagraph("Another text");

        REQUIRE(doc.GetItemsCount() == 3);

        p1->SetText("Modified text");
        img->Resize(200, 200);

        REQUIRE(doc.GetItem(0).GetParagraph()->GetText() == "Modified text");
        REQUIRE(doc.GetItem(1).GetImage()->GetWidth() == 200);

        doc.DeleteItem(1);
        REQUIRE(doc.GetItemsCount() == 2);

        REQUIRE(doc.GetItem(0).GetParagraph()->GetText() == "Modified text");
        REQUIRE(doc.GetItem(1).GetParagraph()->GetText() == "Another text");
    }

    SECTION("Multiple operations with title") 
    {
        doc.SetTitle("Initial Title");
        doc.InsertParagraph("Content");
        doc.SetTitle("Updated Title");
        doc.InsertImage("photo.jpg", 300, 400);
        doc.SetTitle("Final Title");

        REQUIRE(doc.GetTitle() == "Final Title");
        REQUIRE(doc.GetItemsCount() == 2);
    }
}