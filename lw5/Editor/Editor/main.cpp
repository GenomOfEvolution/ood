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

    return EXIT_SUCCESS;
}
