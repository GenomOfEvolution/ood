#pragma once
#include <qmetatype.h>

enum class HandleType
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

Q_DECLARE_METATYPE(HandleType)