#include "stdafx.h"
#include "IStorageManager.h"

using namespace System;
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

IStorageManager::!IStorageManager()
{
	if (_unmanagedObject != nullptr)
	{
		_unmanagedObject->flush();
		delete _unmanagedObject;
		_unmanagedObject = nullptr;
	}
}

IStorageManager::~IStorageManager()
{
	this->!IStorageManager();
	GC::SuppressFinalize(this);
}

void IStorageManager::Flush()
{
	this->getManager()->flush();
}
