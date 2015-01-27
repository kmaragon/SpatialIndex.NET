#include "stdafx.h"

#include "IStorageManager.h"

using namespace Konscious::SpatialIndex;

IStorageManager::IStorageManager() :
	_unmanagedObject(nullptr)
{
}

::SpatialIndex::IStorageManager *IStorageManager::getManager()
{
	if (_unmanagedObject == nullptr)
	{
		_unmanagedObject = this->createUnmanagedManager();
	}
	return _unmanagedObject;
}

IStorageManager::~IStorageManager()
{
	if (_unmanagedObject == nullptr)
	{
		delete _unmanagedObject;
	}
}
