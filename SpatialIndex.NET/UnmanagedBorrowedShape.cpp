#include "stdafx.h"
#include "UnmanagedBorrowedShape.h"
#include "Region.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_shapes;

UnmanagedBorrowedShape::UnmanagedBorrowedShape(const ::SpatialIndex::IShape *realShape)
{
	_shape = realShape;
}

UnmanagedBorrowedShape::~UnmanagedBorrowedShape()
{
	// someone else owns the shape - just let it go... let it go... fseflakjs that song!!!! nooo!!!!
}

Region ^UnmanagedBorrowedShape::BoundingRegion::get()
{
	auto r = new ::SpatialIndex::Region;
	_shape->getMBR(*r);

	return gcnew Region(r);
}

::SpatialIndex::IShape *UnmanagedBorrowedShape::getShape()
{
	return const_cast<::SpatialIndex::IShape *>(_shape);
}

unsigned int UnmanagedBorrowedShape::dimensions()
{
	return _shape->getDimension();
}