#include "stdafx.h"
#include "StorageManagerAccess.h"

using namespace System;
using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::Test;

StorageManagerAccess::StorageManagerAccess(IStorageManager ^manager)
{
	_manager = manager->createUnmanagedManager();
}

StorageManagerAccess::~StorageManagerAccess()
{
	delete _manager;
}

array<Byte> ^StorageManagerAccess::Load(Int64 page)
{
	uint32_t length;
	byte *data;

	_manager->loadByteArray(page, length, &data);
	auto returnArray = gcnew array<Byte>(length);
	for (uint32_t i = 0; i < length; ++i)
	{
		returnArray[i] = data[i];
	}
	return returnArray;
}

Int64 StorageManagerAccess::Store(Int64 page, array<Byte> ^data)
{
	pin_ptr<Byte> rawData = &data[0];
	_manager->storeByteArray(page, data->Length, rawData);
	return page;
}

void StorageManagerAccess::Delete(Int64 page)
{
	_manager->deleteByteArray(page);
}

void StorageManagerAccess::Flush()
{
	_manager->flush();
}
