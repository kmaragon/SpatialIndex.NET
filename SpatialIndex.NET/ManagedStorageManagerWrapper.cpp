#include "stdafx.h"

#include "ManagedStorageManagerWrapper.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_native;

ManagedStorageManagerWrapper::ManagedStorageManagerWrapper(gcroot<IManagedStorageManager ^> realManager)
{
	_managerHandle.attach(realManager);
}

ManagedStorageManagerWrapper::~ManagedStorageManagerWrapper()
{
	_managerHandle.release();
}

void ManagedStorageManagerWrapper::loadByteArray(const ::SpatialIndex::id_type id, uint32_t &len, byte **data)
{
	IManagedStorageManager ^manager = _managerHandle.get();

	auto response = manager->Load(id);
	auto bytes = new byte[response->Length];

	len = response->Length;
	
	pin_ptr<byte> sourceArray = &response[0];
	memcpy(bytes, sourceArray, response->Length);
	*data = bytes;
}

void ManagedStorageManagerWrapper::storeByteArray(::SpatialIndex::id_type &id, const uint32_t len, const byte *const const_data)
{
	IManagedStorageManager ^manager = _managerHandle.get();

	auto dataarray = gcnew cli::array<byte>(len);
	for (uint32_t i = 0; i < len; ++i)
		dataarray[i] = const_data[i];

	if (id == ::SpatialIndex::StorageManager::NewPage)
		id = manager->Store(dataarray);
	else
		id = manager->Store(id, dataarray);
}

void ManagedStorageManagerWrapper::deleteByteArray(const ::SpatialIndex::id_type id)
{
	IManagedStorageManager ^manager = _managerHandle.get();
	manager->Delete(id);
}

void ManagedStorageManagerWrapper::flush()
{
	IManagedStorageManager ^manager = _managerHandle.get();
	manager->Flush();
}