#include "stdafx.h"
#include "Point.h"
#include "SpatialIndexException.h"
#include "Region.h"

using namespace System;
using namespace Konscious::SpatialIndex;

Point::Point(... cli::array<double> ^coords)
{
	try
	{
		pin_ptr<double> coords_ptr = &coords[0];
		_point = new ::SpatialIndex::Point(coords_ptr, coords->Length);
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
}

cli::array<double> ^Point::Coordinates::get()
{
	auto dimensions = _point->getDimension();
	auto returnArray = gcnew cli::array<double>(dimensions);

	for (auto i = 0U; i < dimensions; ++i)
	{
		returnArray->SetValue(_point->getCoordinate(i), (int64_t)i);
	}
	
	return returnArray;
}

int Point::Dimensions::get()
{
	return dimensions();
}

Region ^Point::BoundingRegion::get()
{
	return gcnew Region(this, this);
}

Point::!Point()
{
	delete _point;
	_point = nullptr;
}

Point::~Point()
{
	this->!Point();
	GC::SuppressFinalize(this);
}

Point::Point(::SpatialIndex::Point *point)
{
	_point = point;
}

::SpatialIndex::IShape *Point::getShape()
{
	return _point;
}

unsigned int Point::dimensions()
{
	return _point->getDimension();
}
