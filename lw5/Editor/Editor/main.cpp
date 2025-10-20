#include <iostream>
#include "Document/HtmlDocument/HtmlDocument.h"
#include "History/History.h"
#include "Saver/HtmlSaver.h"
#include "DocumentItem/Paragraph/Paragraph.h"

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
    doc.Save("D:\\test\\");

    return EXIT_SUCCESS;
}
