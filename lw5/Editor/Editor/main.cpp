#include "Document/HtmlDocument/HtmlDocument.h"
#include "History/History.h"
#include "Saver/HtmlSaver.h"
#include "DocumentItem/Image/IImage.h"
#include "Command/MergableCommand/ResizeImageCommand/ResizeImageCommand.h"

#include <iostream>
#include <memory>

int main()
{
    HtmlSaver saver;
    History history;

    HtmlDocument doc(
        std::make_unique<History>(std::move(history)),
        std::make_unique<HtmlSaver>(std::move(saver))
    );

    doc.InsertParagraph("<Cool text> &\n 'Comma text'\n");
    doc.InsertParagraph("Hello1 world");
    doc.InsertParagraph("Hello2 world");
    doc.InsertParagraph("Hello 3world");
    doc.InsertImage("D:/gneg.jpg", 800, 600, 1);

    doc.SetTitle("Hello world");
    doc.SetTitle("Hello worl3");

    auto item = doc.GetItem(1);
    auto img = item.GetImage();
    
    auto cmd = std::make_unique<ResizeImageCommand>(img, 400, 28);
    cmd->Execute();

    doc.Save("D:\\test\\");

    return EXIT_SUCCESS;
}
