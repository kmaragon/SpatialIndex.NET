#include "stdafx.h"
#include "UnmanagedOwnedShape.h"
#include "Region.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_shapes;

UnmanagedOwnedShape::UnmanagedOwnedShape(const ::SpatialIndex::IShape *realShape)
{
	_shape = realShape;
}

UnmanagedOwnedShape::!UnmanagedOwnedShape()
{
	delete _shape;
	_shape = nullptr;
}

UnmanagedOwnedShape::~UnmanagedOwnedShape()
{
	this->!UnmanagedOwnedShape();
}

Region ^UnmanagedOwnedShape::BoundingRegion::get()
{
	auto r = new ::SpatialIndex::Region;
	_shape->getMBR(*r);

	return gcnew Region(r);
}

::SpatialIndex::IShape *UnmanagedOwnedShape::getShape()
{
	return const_cast<::SpatialIndex::IShape *>(_shape);
}

unsigned int UnmanagedOwnedShape::dimensions()
{
	return _shape->getDimension();
}