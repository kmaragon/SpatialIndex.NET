#include "stdafx.h"
#include "InternalHelpers.h"
#include "UnmanagedBorrowedShape.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_helpers;
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

IShape ^InternalHelpers::getManagedFromNative(::SpatialIndex::IShape *shape)
{
	/// TODO: update this when a managed shape exists
	return gcnew UnmanagedBorrowedShape(shape);
}