#include <QApplication>
#include <memory>
#include "Views/MainView/MainView.h"

#include "Models/Document/DocumentModel.h"
#include "Models/History/CommandHistory.h"
#include "Models/DocumentSerializer/XmlSerializer.h"

#include "Controllers/DocumentController/DocumentController.h"

using namespace UI;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app-icon.ico"));

    auto history = std::make_shared<CommandHistory>();
    auto saver = std::make_shared<XmlSerializer>();
    auto doc = std::make_shared<DocumentModel>(history, saver);

    saver->SetDocument(doc);

    auto docController = new DocumentController(doc, history);

    MainView view(docController);

    view.show();

    return app.exec();
}