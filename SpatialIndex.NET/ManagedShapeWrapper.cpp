#include "stdafx.h"
#include "ManagedShapeWrapper.h"
#include "InternalHelpers.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_native;
using namespace Konscious::SpatialIndex::_helpers;
using namespace System;
using namespace System::IO;
using namespace System::Runtime::Serialization::Formatters::Binary;

ManagedShapeWrapper::ManagedShapeWrapper(gcroot<IManagedShapeBase ^> realShape)
{
	_shapeHandle = realShape;
}

ManagedShapeWrapper::~ManagedShapeWrapper()
{
	_shapeHandle.release();
}


bool ManagedShapeWrapper::intersectsShape(const ::SpatialIndex::IShape& in) const
{
	return _shapeHandle.get()->Intersects(InternalHelpers::getManagedFromNative(&in));
}

bool ManagedShapeWrapper::containsShape(const ::SpatialIndex::IShape& in) const
{
	return _shapeHandle.get()->Contains(InternalHelpers::getManagedFromNative(&in));
}

bool ManagedShapeWrapper::touchesShape(const ::SpatialIndex::IShape& in) const
{
	return _shapeHandle.get()->Touches(InternalHelpers::getManagedFromNative(&in));
}

void ManagedShapeWrapper::getCenter(::SpatialIndex::Point& out) const
{
	auto pt = _shapeHandle.get()->Center;
	out = *pt->_point;
}

uint32_t ManagedShapeWrapper::getDimension() const
{
	return _shapeHandle.get()->Dimensions;
}

void ManagedShapeWrapper::getMBR(::SpatialIndex::Region& out) const
{
	auto rg = _shapeHandle.get()->BoundingRegion;
	out = *rg->_region;
}

double ManagedShapeWrapper::getArea() const
{
	return _shapeHandle.get()->Area;
}

double ManagedShapeWrapper::getMinimumDistance(const ::SpatialIndex::IShape& in) const
{
	return _shapeHandle.get()->MinimumDistanceFrom(InternalHelpers::getManagedFromNative(&in));
}

uint32_t ManagedShapeWrapper::getByteArraySize()
{
	auto formatter = gcnew BinaryFormatter();

	auto nStream = gcnew MemoryStream();
	formatter->Serialize(nStream, _shapeHandle.get());
	return (uint32_t) nStream->Length;
}

void ManagedShapeWrapper::loadFromByteArray(const byte* data)
{
	auto formatter = gcnew BinaryFormatter();

	// the libspatialindex method is just asking for a problem here without a length
	auto len = *((int *)data);
	auto rdstream = gcnew UnmanagedMemoryStream(const_cast<byte*>(data) + sizeof(int), len);

	auto newObj = formatter->Deserialize(rdstream);
	auto newShape = dynamic_cast<IManagedShapeBase ^>(newObj);

	if (newShape != nullptr)
	{
		_shapeHandle.release();
		_shapeHandle = newShape;
	}
}

void ManagedShapeWrapper::storeToByteArray(byte** data, uint32_t& length)
{
	auto formatter = gcnew BinaryFormatter();

	auto nStream = gcnew MemoryStream();
	formatter->Serialize(nStream, _shapeHandle.get());

	length = (uint32_t) nStream->Length;
	
	*data = new byte[length + sizeof(int)];
	pin_ptr<byte> starting = &nStream->ToArray()[0];
	*((int *)(*data)) = length;
	memcpy((*data + sizeof(int)), starting, (size_t)nStream->Length);
}