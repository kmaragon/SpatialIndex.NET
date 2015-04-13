#include "stdafx.h"
#include <msclr\marshal_cppstd.h>
#include "MemoryStorageManager.h"
#include "SpatialIndexException.h"

using namespace Konscious::SpatialIndex;
using namespace ::Tools;
using namespace System;

MemoryStorageManager::MemoryStorageManager()
{
}

MemoryStorageManager::!MemoryStorageManager()
{
}

MemoryStorageManager::~MemoryStorageManager()
{
	this->!MemoryStorageManager();
}

::SpatialIndex::IStorageManager *MemoryStorageManager::createUnmanagedManager()
{
	PropertySet properties;

	try
	{
		return ::SpatialIndex::StorageManager::returnMemoryStorageManager(properties);
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
}
