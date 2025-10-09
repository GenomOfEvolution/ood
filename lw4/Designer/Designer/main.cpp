#include "Designer/Designer.h"
#include "ShapeFactory/ShapeFactory.h"
#include "Canvas/SVGCanvas/SVGCanvas.h"
#include "Painter/Painter.h"
#include "Client/Client.h"

int main()
{
    ShapeFactory shapeFactory;
    std::unique_ptr<IDesigner> designer = std::make_unique<Designer>(shapeFactory);
    std::unique_ptr<ICanvas> canvas = std::make_unique<CanvasSVG>("output.svg");
    Painter painter;

    Client client(std::move(canvas));
    client.AskForDraft(*designer);
    client.AskForPaintingDraft(painter);

    return EXIT_SUCCESS;
}

