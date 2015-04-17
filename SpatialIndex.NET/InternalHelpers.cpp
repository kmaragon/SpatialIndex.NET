#include "stdafx.h"
#include "InternalHelpers.h"
#include "UnmanagedBorrowedShape.h"
#include "UnmanagedOwnedShape.h"
#include "ManagedShapeWrapper.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_helpers;
using namespace Konscious::SpatialIndex::_native;
using namespace Konscious::SpatialIndex::_shapes;
using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Runtime::Serialization::Formatters::Binary;

const byte __buffer_signature[] = { "\x1C\x01\x7C\x51\x01\x15\x01\x05" };
const size_t __buffer_signature_length = 8;

InternalHelpers::InternalHelpers()
{
}

KeyValuePair<IShape ^, Object ^> InternalHelpers::pairFromData(const ::SpatialIndex::IData &in)
{
	uint32_t dataLen;
	byte *realData;

	in.getData(dataLen, &realData);

	try
	{
		IShape ^rtValue;

		byte *valueBuffer;
		int valueLen;
		BinaryFormatter ^formatter = gcnew BinaryFormatter();

		if (dataLen >= 10 && !memcmp(realData, __buffer_signature, __buffer_signature_length))
		{
			// this is probably a managed block
			uint32_t shapeSize = *((uint32_t*)&realData[__buffer_signature_length]);

			auto shapeOffset = sizeof(uint32_t) + __buffer_signature_length;
			valueLen = dataLen - (shapeSize + (int)shapeOffset);
			valueBuffer = &realData[shapeSize + shapeOffset];

			rtValue = (IShape ^)formatter->Deserialize(gcnew UnmanagedMemoryStream(realData + shapeOffset, (Int64)shapeSize));
		}
		else
		{
			::SpatialIndex::IShape *realShape;

			in.getShape(&realShape);
			// turns out this is new'ed up
			rtValue = gcnew UnmanagedOwnedShape(realShape);

			valueBuffer = realData;
			valueLen = dataLen;
		}

		auto valueStream = gcnew UnmanagedMemoryStream(valueBuffer, valueLen);

		try
		{
			Object ^value = formatter->Deserialize(valueStream);
			rtValue->Id = in.getIdentifier();
			return KeyValuePair<IShape ^, Object ^>(rtValue, value);
		}
		finally
		{
			delete valueStream;
		}
	}
	finally
	{
		delete[] realData;
	}
}

generic<typename TValue>
array<Byte> ^InternalHelpers::dataFromPair(IShape ^shape, TValue value)
{
	BinaryFormatter ^formatter = gcnew BinaryFormatter();
	auto serialStream = gcnew MemoryStream();

	// serialize the value
	formatter->Serialize(serialStream, value);

	auto managedShape = dynamic_cast<IManagedShapeBase ^>(shape);
	if (managedShape == nullptr)
	{
		return serialStream->ToArray();
	}

	pin_ptr<Byte> serialBuffer = &serialStream->ToArray()[0];

	auto shapeStream = gcnew MemoryStream();
	formatter->Serialize(shapeStream, managedShape);

	pin_ptr<Byte> managedBuffer = &shapeStream->ToArray()[0];

	// so we have a managed shape
	auto finalArray = gcnew array<Byte>((int)(__buffer_signature_length + sizeof(uint32_t) + shapeStream->Position + serialStream->Position));
	pin_ptr<Byte> dstArray = &finalArray[0];

	byte *dstptr = (Byte*)dstArray;
	memcpy(dstptr, __buffer_signature, __buffer_signature_length);
	dstptr += __buffer_signature_length;

	*((uint32_t *)dstptr) = (uint32_t)shapeStream->Position;
	dstptr += sizeof(uint32_t);

	memcpy(dstptr, (Byte*)managedBuffer, (size_t)shapeStream->Position);
	dstptr += shapeStream->Position;

	memcpy(dstptr, (Byte*)serialBuffer, (size_t)serialStream->Position);

	// free my streams
	delete serialStream;
	delete shapeStream;

	return finalArray;
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

CountVisitor::CountVisitor()
{
	_count = 0;
}

CountVisitor::~CountVisitor()
{
}

int CountVisitor::getResultCount() const
{
	return _count;
}

void CountVisitor::visitNode(const ::SpatialIndex::INode &in)
{
}

void CountVisitor::visitData(const ::SpatialIndex::IData &in)
{
	++_count;
}

void CountVisitor::visitData(std::vector<const ::SpatialIndex::IData*> &v)
{
	_count += v.size();
}
