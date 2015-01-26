#include "stdafx.h"
#include "IShape.h"

using namespace Konscious::SpatialIndex;

IShape::IShape()
{
	_realShape = nullptr;
}

IShape::~IShape()
{
	if (_realShape != nullptr)
	{
		delete _realShape;
	}
}
