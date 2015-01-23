#include "stdafx.h"
#include "IManagedStorageManager.h"
#include "ManagedStorageManagerWrapper.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_native;

::SpatialIndex::IStorageManager *IManagedStorageManager::createUnmanagedManager()
{
	return new ManagedStorageManagerWrapper(this);
}
