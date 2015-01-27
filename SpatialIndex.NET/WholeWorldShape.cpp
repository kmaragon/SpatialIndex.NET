#include "stdafx.h"
#include "WholeWorldShape.h"

using namespace Konscious::SpatialIndex::_shapes;

WholeWorldShape::WholeWorldShape(int dimensions)
{
	_dimensions = dimensions;
}


WholeWorldShape::~WholeWorldShape()
{
}

bool WholeWorldShape::intersectsShape(const ::SpatialIndex::IShape& in) const
{
	return true;
}

bool WholeWorldShape::containsShape(const ::SpatialIndex::IShape& in) const
{
	return true;
}

bool WholeWorldShape::touchesShape(const ::SpatialIndex::IShape& in) const
{
	return true;
}

void WholeWorldShape::getCenter(::SpatialIndex::Point& out) const
{
	double *point = (double*)alloca(_dimensions * sizeof(double));
	for (int i = 0; i < _dimensions; ++i)
		point[i] = 0;
	out = ::SpatialIndex::Point(point, _dimensions);
}

uint32_t WholeWorldShape::getDimension() const
{
	return _dimensions;
}

void WholeWorldShape::getMBR(::SpatialIndex::Region& out) const
{
	double *pointlow = (double*)alloca(_dimensions * sizeof(double));
	double *pointhigh = (double*)alloca(_dimensions * sizeof(double));
	
	for (int i = 0; i < _dimensions; ++i)
	{
		pointlow[i] = DBL_MIN;
		pointhigh[i] = DBL_MAX;
	}
	out = ::SpatialIndex::Region(::SpatialIndex::Point(pointlow, _dimensions), ::SpatialIndex::Point(pointhigh, _dimensions));
}

double WholeWorldShape::getArea() const
{
	return DBL_MAX;
}

double WholeWorldShape::getMinimumDistance(const ::SpatialIndex::IShape& in) const
{
	return 0;
}

uint32_t WholeWorldShape::getByteArraySize()
{
	return 1;
}

void WholeWorldShape::loadFromByteArray(const byte* data)
{
}

void WholeWorldShape::storeToByteArray(byte** data, uint32_t& length)
{
	*data = new byte[1] { 0 };
	length = 1;
}
