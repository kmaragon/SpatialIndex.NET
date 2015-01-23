#include "stdafx.h"

#include "IStorageManager.h"

using namespace Konscious::SpatialIndex;

IStorageManager::IStorageManager() :
	unmanagedObject(nullptr)
{
}

IStorageManager::~IStorageManager()
{
	if (unmanagedObject == nullptr)
	{
		delete unmanagedObject;
	}
}
