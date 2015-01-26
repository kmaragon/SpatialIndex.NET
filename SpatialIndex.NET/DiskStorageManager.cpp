#include "stdafx.h"
#include <msclr\marshal_cppstd.h>
#include "DiskStorageManager.h"
#include "SpatialIndexException.h"

using namespace Konscious::SpatialIndex;
using namespace System;
using namespace ::Tools;

DiskStorageManager::DiskStorageManager(String ^filename)
{
	_initialized = false;
	_props = new _dsm_internal_data;
	_props->overwrite = true;
	this->Filename = filename;
}

DiskStorageManager::~DiskStorageManager()
{
	delete _props;
}

#define CheckInitialized() \
	if (_initialized) \
	    throw gcnew InvalidOperationException("The file has already been opened and initialized")

bool DiskStorageManager::Overwrite::get()
{
	return _props->overwrite;
}

void DiskStorageManager::Overwrite::set(bool val)
{
	CheckInitialized();
	_props->overwrite = val;
}

System::String ^DiskStorageManager::Filename::get()
{
	return gcnew String(_props->filename.c_str(), 0, (int)_props->filename.length());
}

void DiskStorageManager::Filename::set(System::String ^val)
{
	CheckInitialized();
	if (val != nullptr)
	{
		_props->filename = msclr::interop::marshal_as<std::string>(val);
	}
	else
	{
		_props->filename = "";
	}
}

System::UInt32 DiskStorageManager::PageSize::get()
{
	return _props->pageSize;
}

void DiskStorageManager::PageSize::set(System::UInt32 val)
{
	CheckInitialized();
	_props->pageSize = val;
}

System::String ^DiskStorageManager::DatafileSuffix::get()
{
	return gcnew String(_props->dfsuffix.c_str(), 0, (int)_props->dfsuffix.length());
}

void DiskStorageManager::DatafileSuffix::set(System::String ^val)
{
	CheckInitialized();
	if (val != nullptr)
	{
		_props->dfsuffix = msclr::interop::marshal_as<std::string>(val);
	}
	else
	{
		_props->dfsuffix = "";
	}
}

System::String ^DiskStorageManager::IndexfileSuffix::get()
{
	return gcnew String(_props->ifSuffix.c_str(), 0, (int)_props->ifSuffix.length());
}

void DiskStorageManager::IndexfileSuffix::set(System::String ^val)
{
	CheckInitialized();
	if (val != nullptr)
	{
		_props->ifSuffix = msclr::interop::marshal_as<std::string>(val);
	}
	else
	{
		_props->ifSuffix = "";
	}
}

void DiskStorageManager::fillProperties(PropertySet &fillSet)
{
	setStringVariant(fillSet, "FileName", _props->filename);
	setStringVariant(fillSet, "FileNameIdx", _props->ifSuffix);
	setStringVariant(fillSet, "FileNameDat", _props->dfsuffix);
	setBoolVariant(fillSet, "Overwrite", _props->overwrite);
	setULongVariant(fillSet, "PageSize", _props->pageSize);
}

void DiskStorageManager::setProperty(PropertySet &properties, const char *name, const Variant &value)
{
	try
	{
		properties.setProperty(name, value);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
}

void DiskStorageManager::setStringVariant(PropertySet &properties, const char *prop, const std::string &value)
{
	if (!value.length())
	{
		return;
	}

	Variant var;
	var.m_varType = Tools::VT_PCHAR;
	var.m_val.pcVal = const_cast<char*>(value.c_str());

	setProperty(properties, prop, var);
}

void DiskStorageManager::setBoolVariant(PropertySet &properties, const char *prop, bool value)
{
	Variant var;
	var.m_varType = Tools::VT_BOOL;
	var.m_val.blVal = value;

	setProperty(properties, prop, var);
}

void DiskStorageManager::setULongVariant(PropertySet &properties, const char *prop, uint32_t value)
{
	Variant var;
	var.m_varType = Tools::VT_ULONG;
	var.m_val.ulVal = value;

	setProperty(properties, prop, var);
}

::SpatialIndex::IStorageManager *DiskStorageManager::createUnmanagedManager()
{
	_initialized = true;
	PropertySet properties;
	fillProperties(properties);

	try
	{
		return ::SpatialIndex::StorageManager::returnDiskStorageManager(properties);
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
