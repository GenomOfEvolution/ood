#include <QApplication>
#include <memory>
#include "Views/MainView/MainView.h"

#include "Models/Document/DocumentModel.h"
#include "Models/History/CommandHistory.h"
#include "Models/DocumentSerializer/XmlSerializer.h"
#include "Models/ImageStorage/ImageStorage.h"
#include "Models/Shared/ItemPreviewDTO.h"
#include "Models/Shared/HandleType.h"
#include "Models/Selection/DocumentSelection.h"

#include "Controllers/DocumentController/DocumentController.h"

using namespace UI;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app-icon.ico"));

    qRegisterMetaType<DocItemPreview>();
    qRegisterMetaType<HandleType>("HandleType");

    auto selection = std::make_shared<DocumentSelection>();
    auto imgStorage = std::make_shared<ImageStorage>();
    auto history = std::make_shared<CommandHistory>();
    auto saver = std::make_shared<XmlSerializer>(imgStorage);
    auto doc = std::make_shared<DocumentModel>(history, saver);
    
    selection->SetDocument(doc);
    saver->SetDocument(doc);

    auto docController = new DocumentController(doc, history, imgStorage, selection);

    MainView view(docController);

    view.show();

    return app.exec();
}