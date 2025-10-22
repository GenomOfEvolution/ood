#include "Document/HtmlDocument/HtmlDocument.h"
#include "History/History.h"
#include "Saver/HtmlSaver.h"
#include "DocumentItem/Image/IImage.h"
#include "Command/InsertImageCommand/InsertImageCommand.h"

#include <iostream>
#include <memory>

int main()
{
    auto saver = std::make_shared<HtmlSaver>();
    auto history = std::make_shared<History>();

    HtmlDocument doc(history, saver);

    doc.InsertParagraph("<Cool text> &\n 'Comma text'\n");
    doc.InsertParagraph("Hello1 world");
    doc.InsertParagraph("Hello2 world");
    doc.InsertParagraph("Hello 3world");

    std::optional<size_t> pos = 1;

    auto cmd = std::make_unique<InsertImageCommand>(doc, *saver, pos, 800, 600, "D:/Sword.png");
    cmd->Execute();

    doc.SetTitle("Hello world");
    doc.SetTitle("Hello worl3");

    auto item = doc.GetItem(1);
    auto img = item.GetImage();

    doc.Save("D:\\test\\");

    return EXIT_SUCCESS;
}
