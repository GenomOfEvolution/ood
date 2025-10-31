#include "Slide/Slide.h"
#include "Canvas/CanvasSVG/CanvasSVG.h"
#include "Shape/Ellipse/Ellipse.h"
#include "Shape/Rectangle/Rectangle.h"
#include "Shape/Triangle/Triangle.h"
#include "ShapeGroup/GroupShape.h"
#include <iostream>

std::shared_ptr<IShape> MakeCat()
{
	const auto head = std::make_shared<CEllipse>(PointD{ 300, 100 }, 80, 70);
	head->GetFillStyle()->SetColor(0xfc8403ff);
	head->GetOutlineStyle()->Disable();

	const auto nose = std::make_shared<CTriangle>(PointD{ 295, 120 }, PointD{ 300, 125 }, PointD{ 305, 120 });
	nose->GetFillStyle()->SetColor(0xfc4eb4);
	nose->GetOutlineStyle()->SetColor(0xffffff);
	nose->GetOutlineStyle()->SetThickness(1);

	const auto leftEar = std::make_shared<CTriangle>(PointD{ 230, 70 }, PointD{ 250, 10 }, PointD{ 270, 50 });
	leftEar->GetFillStyle()->SetColor(0xfc8403ff);
	leftEar->GetOutlineStyle()->Disable();
	leftEar->GetOutlineStyle()->Enable();
	leftEar->GetOutlineStyle()->SetThickness(5);
	leftEar->GetOutlineStyle()->SetColor(0xbd550bff);

	const auto rightEar = std::make_shared<CTriangle>(PointD{ 330, 50 }, PointD{ 350, 10 }, PointD{ 370, 70 });
	rightEar->GetFillStyle()->SetColor(0xfc8403ff);
	rightEar->GetOutlineStyle()->Disable();
	rightEar->GetOutlineStyle()->Enable();
	rightEar->GetOutlineStyle()->SetThickness(5);
	rightEar->GetOutlineStyle()->SetColor(0xbd550bff);

	const auto mouth = std::make_shared<CTriangle>(PointD{ 295, 130 }, PointD{ 300, 135 }, PointD{ 305, 130 });
	mouth->GetFillStyle()->SetColor(0x000000);
	mouth->GetOutlineStyle()->SetColor(0xffffff);
	mouth->GetOutlineStyle()->SetThickness(2);

	const auto leftSclera = std::make_shared<CEllipse>(PointD{ 270, 90 }, 15, 20);
	leftSclera->GetFillStyle()->SetColor(0xffffff);
	leftSclera->GetOutlineStyle()->Disable();

	const auto leftPupil = std::make_shared<CEllipse>(PointD{ 270, 95 }, 10, 15);
	leftPupil->GetFillStyle()->SetColor(0xeb4efc);
	leftPupil->GetOutlineStyle()->Disable();

	const auto leftEye = std::make_shared<GroupShape>();
	leftEye->InsertShape(leftSclera, 0);
	leftEye->InsertShape(leftPupil, 1);

	const auto rightSclera = std::make_shared<CEllipse>(PointD{ 330, 90 }, 15, 20);
	rightSclera->GetFillStyle()->SetColor(0xffffff);
	rightSclera->GetOutlineStyle()->Disable();

	const auto rightPupil = std::make_shared<CEllipse>(PointD{ 330, 95 }, 10, 15);
	rightPupil->GetFillStyle()->SetColor(0xeb4efc);
	rightPupil->GetOutlineStyle()->Disable();

	const auto rightEye = std::make_shared<GroupShape>();
	rightEye->InsertShape(rightSclera, 0);
	rightEye->InsertShape(rightPupil, 1);

	const auto body = std::make_shared<CEllipse>(PointD{ 420, 200 }, 150, 60);
	body->GetFillStyle()->SetColor(0xfc8403ff);
	body->GetOutlineStyle()->Disable();

	const auto tail = std::make_shared<CRectangle>(PointD{ 520, 50 }, 30, 150);
	tail->GetFillStyle()->SetColor(0xfc8403ff);
	tail->GetOutlineStyle()->Enable();
	tail->GetOutlineStyle()->SetThickness(5);
	tail->GetOutlineStyle()->SetColor(0xbd550bff);

	auto cutieKitty = std::make_shared<GroupShape>();
	cutieKitty->InsertShape(leftEar, 0);
	cutieKitty->InsertShape(rightEar, 1);
	cutieKitty->InsertShape(head, 2);
	cutieKitty->InsertShape(leftEye, 3);
	cutieKitty->InsertShape(rightEye, 4);
	cutieKitty->InsertShape(nose, 5);
	cutieKitty->InsertShape(mouth, 6);
	cutieKitty->InsertShape(tail, 7);
	cutieKitty->InsertShape(body, 8);

	cutieKitty->GetFillStyle()->SetColor(0x00000000);

	return cutieKitty;
}

int main()
{
    CanvasSvg canvas;
    try
    {
		auto slide = std::make_shared<Slide>();
		auto& shapes = slide->GetShapes();
		shapes.InsertShape(MakeCat(), 0);

        slide->Draw(canvas);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
