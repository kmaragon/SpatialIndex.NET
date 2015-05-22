#include "stdafx.h"
#include "InternalHelpers.h"
#include "SpatialIndexData.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_helpers;
using namespace System;

generic<typename TValueType>
SpatialIndexData<TValueType>::!SpatialIndexData()
{
	if (_shape != nullptr)
	{
		// this whole point of this is to expose
		// the shape and data for later use
		//
		// so no delete - just deref here
		_shape = nullptr;
		_data = TValueType();
	}
}

generic<typename TValueType>
SpatialIndexData<TValueType>::~SpatialIndexData()
{
	this->!SpatialIndexData();
}

generic<typename TValueType>
IShape ^SpatialIndexData<TValueType>::Shape::get()
{
	checkInitialize();
	return _shape;
}

generic<typename TValueType>
TValueType SpatialIndexData<TValueType>::Value::get()
{
	checkInitialize();
	return _data;
}

generic<typename TValueType>
SpatialIndexData<TValueType>::SpatialIndexData(const ::SpatialIndex::IData &dat)
	: _idata(dat)
{
}

generic<typename TValueType>
void SpatialIndexData<TValueType>::checkInitialize()
{
	if (_shape == nullptr)
	{
		auto kvp = InternalHelpers::pairFromData(_idata);
		_shape = kvp.Key;
		_data = (TValueType)kvp.Value;
	}
}
