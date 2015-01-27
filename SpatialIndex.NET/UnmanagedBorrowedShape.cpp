#include "stdafx.h"
#include "UnmanagedBorrowedShape.h"

using namespace Konscious::SpatialIndex::_shapes;

UnmanagedBorrowedShape::UnmanagedBorrowedShape(::SpatialIndex::IShape *realShape)
{
	_shape = realShape;
}

UnmanagedBorrowedShape::~UnmanagedBorrowedShape()
{
	// someone else owns the shape - just let it go... let it go... fseflakjs that song!!!! nooo!!!!
}

::SpatialIndex::IShape *UnmanagedBorrowedShape::getShape()
{
	return _shape;
}