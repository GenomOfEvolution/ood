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
#include "Services/QtDialogService/QtDialogService.h"


// TODO: сделать активную модель, разгрузить контроллер
// TODO: сделать поодержку нескольких окон
// TODO: пофиксить визуальные баги с фигурами и перерисовкой - оптимизировать работу команд
// TODO: сделать ресайз сразу, а не по дабл клику
// TODO: сделать еще один бизнес слой для модели
// TODO: выделить отдельный слой под рабочую область
// TODO: клик по не выделенной фигуре на ее рамку не должен выделять фигуру - прим с эллипсом

using namespace UI;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app-icon.ico"));

    qRegisterMetaType<DocItemPreview>();
    qRegisterMetaType<HandleType>("HandleType");

    const Rect docWorkArea = Rect(0, 0, 1200, 500);

    auto selection = std::make_shared<DocumentSelection>();
    auto imgStorage = std::make_shared<ImageStorage>();
    auto history = std::make_shared<CommandHistory>();
    auto saver = std::make_shared<XmlSerializer>(imgStorage);
    auto doc = std::make_shared<DocumentModel>(history, saver, docWorkArea);
    auto dialogService = std::make_shared<QtDialogService>();
    
    selection->SetDocument(doc);
    saver->SetDocument(doc);

    auto docController = new DocumentController(
        doc,
        history,
        imgStorage, 
        selection, 
        dialogService);

    MainView view(docController);

    view.show();

    return app.exec();
}