#include <iostream>
#include <memory>

#include "Shape/Picture.h"
#include "gfx/Canvas.h"
#include "Command/CommandProcessor.h"
#include "Command/AddShapeCommand.h"
#include "Command/MoveShapeCommand.h"
#include "Command/MovePictureCommand.h"
#include "Command/DeleteShapeCommand.h"
#include "Command/ListCommand.h"
#include "Command/ChangeColorCommand.h"
#include "Command/ChangeShapeCommand.h"
#include "Command/DrawShapeCommand.h"
#include "Command/DrawPictureCommand.h"
#include "Command/HelpCommand.h"
#include "Command/ExitCommand.h"

using namespace shapes;
using namespace gfx;
using namespace UI;

int main()
{
	Picture picture;
    Canvas canvas;
	CommandProcessor menu(std::cin, std::cout);

	menu.AddItem("help", "show this menu", std::make_unique<HelpCommand>(menu));
	menu.AddItem("exit", "exit from program", std::make_unique<ExitCommand>(menu));
	menu.AddItem("AddShape", "AddShape <id> <color> <type> <params>", std::make_unique<AddShapeCommand>(picture));
	menu.AddItem("MoveShape", "MoveShape <id> <dx> <dy>", std::make_unique<MoveShapeCommand>(picture));
	menu.AddItem("MovePicture", "MovePicture <dx> <dy>", std::make_unique<MovePictureCommand>(picture));
	menu.AddItem("DeleteShape", "DeleteShape <id>", std::make_unique<DeleteShapeCommand>(picture));
	menu.AddItem("List", "shows all shapes data", std::make_unique<ListCommand>(picture));
	menu.AddItem("ChangeColor", "ChangeColor <id> <color>", std::make_unique<ChangeColorCommand>(picture));
	menu.AddItem("ChangeShape", "ChangeShape <id> <type> <params>", std::make_unique<ChangeShapeCommand>(picture));
	menu.AddItem("DrawShape", "DrawShape <id>", std::make_unique<DrawShapeCommand>(picture, &canvas));
	menu.AddItem("DrawPicture", "draws all shapes on canvas", std::make_unique<DrawPictureCommand>(picture, &canvas));
	
	menu.Run();

	return EXIT_SUCCESS;
}