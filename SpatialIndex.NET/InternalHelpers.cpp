#include "stdafx.h"
#include "InternalHelpers.h"
#include "UnmanagedBorrowedShape.h"
#include "ManagedShapeWrapper.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_helpers;
using namespace Konscious::SpatialIndex::_native;
using namespace Konscious::SpatialIndex::_shapes;
using namespace System;
using namespace System::Collections::Generic;

InternalHelpers::InternalHelpers()
{
}

KeyValuePair<IShape ^, array<byte> ^> InternalHelpers::pairFromData(const ::SpatialIndex::IData &in)
{
	::SpatialIndex::IShape *realShape;
	uint32_t dataLen;
	byte *realData;

	in.getData(dataLen, &realData);
	auto data = gcnew array<byte>(dataLen);
	pin_ptr<byte> data_tag = &data[0];
	memcpy((byte *)data_tag, realData, dataLen);

	in.getShape(&realShape);
	IShape ^retShape = InternalHelpers::getManagedFromNative(realShape);
	retShape->Id = in.getIdentifier();

	return KeyValuePair<IShape ^, array<byte> ^>(retShape, data);
}

IShape ^InternalHelpers::getManagedFromNative(const ::SpatialIndex::IShape *shape)
{
	// is this worth the cost?
	auto wrapped = dynamic_cast<const ManagedShapeWrapper *>(shape);
	if (wrapped != nullptr)
	{
		return wrapped->_shapeHandle.get();
	}
	return gcnew UnmanagedBorrowedShape(shape);
}