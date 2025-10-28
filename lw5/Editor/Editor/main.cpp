#include "Document/HtmlDocument/HtmlDocument.h"
#include "History/History.h"
#include "Saver/HtmlSaver.h"
#include "Menu/Menu.h"

#include <iostream>
#include <memory>

int main()
{
    auto saver = std::make_shared<HtmlSaver>();
    auto history = std::make_shared<History>();
    auto document = std::make_unique<HtmlDocument>(history, saver);

    Menu menu(std::move(saver), std::move(history), std::move(document));

    menu.Run();

    return EXIT_SUCCESS;
}