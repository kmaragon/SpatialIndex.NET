#include "stdafx.h"
#include "RTree.h"

using namespace Konscious::SpatialIndex;
using namespace ::Tools;
using namespace System;

#define DEFAULT_FILL_FACTOR 0.7
#define DEFAULT_NEAR_MIN_OVERLAP_FACTOR 32U
#define DEFAULT_SPLIT_DIST_FACTOR 0.4
#define DEFAULT_REINSERT_FACTOR 0.3
#define DEFAULT_INDEX_CAPACITY 100U
#define DEFAULT_INDEX_POOL_CAPACITY 100U
#define DEFAULT_LEAF_CAPACITY 100U
#define DEFAULT_LEAF_POOL_CAPACITY 100U
#define DEFAULT_REGION_POOL_CAPACITY 1000U
#define DEFAULT_POINT_POOL_CAPACITY 500U
#define DEFAULT_DIMENSION 2U
#define DEFAULT_TIGHT_MBRS true
#define DEFAULT_TREE_VARIANT (int)RTreeVariant::RStar

RTreeOptions::RTreeOptions() :
	RTreeOptions(RTreeOptions::NewIndex)
{
}

RTreeOptions::RTreeOptions(System::Int64 indexIdentifier)
{
	_set = new PropertySet;
	if (indexIdentifier != NewIndex)
		IndexIdentifier = indexIdentifier;
}

RTreeOptions::!RTreeOptions()
{
	delete _set;
	_set = nullptr;
}

RTreeOptions::~RTreeOptions()
{
	this->!RTreeOptions();
}

double RTreeOptions::FillFactor::get()
{
	return getOption(*_set, "FillFactor", DEFAULT_FILL_FACTOR);
}

void RTreeOptions::FillFactor::set(double value)
{
	setOption(*_set, "FillFactor", value);
}

unsigned int RTreeOptions::NearMinimumOverlapFactor::get()
{
	return getOption(*_set, "NearMinimumOverlapFactor", DEFAULT_NEAR_MIN_OVERLAP_FACTOR);
}

void RTreeOptions::NearMinimumOverlapFactor::set(unsigned int value)
{
	setOption(*_set, "NearMinimumOverlapFactor", value);
}

double RTreeOptions::SplitDistributionFactor::get()
{
	return getOption(*_set, "SplitDistributionFactor", DEFAULT_SPLIT_DIST_FACTOR);
}

void RTreeOptions::SplitDistributionFactor::set(double value)
{
	setOption(*_set, "SplitDistributionFactor", value);
}

double RTreeOptions::ReinsertFactor::get()
{
	return getOption(*_set, "ReinsertFactor", DEFAULT_REINSERT_FACTOR);
}

void RTreeOptions::ReinsertFactor::set(double value)
{
	setOption(*_set, "ReinsertFactor", value);
}

unsigned int RTreeOptions::IndexCapacity::get()
{
	return getOption(*_set, "IndexCapacity", DEFAULT_INDEX_CAPACITY);
}

void RTreeOptions::IndexCapacity::set(unsigned int value)
{
	setOption(*_set, "IndexCapacity", value);
}

unsigned int RTreeOptions::IndexPoolCapacity::get()
{
	return getOption(*_set, "IndexPoolCapacity", DEFAULT_INDEX_POOL_CAPACITY);
}

void RTreeOptions::IndexPoolCapacity::set(unsigned int value)
{
	setOption(*_set, "IndexPoolCapacity", value);
}

unsigned int RTreeOptions::LeafCapacity::get()
{
	return getOption(*_set, "LeafCapacity", DEFAULT_LEAF_CAPACITY);
}

void RTreeOptions::LeafCapacity::set(unsigned int value)
{
	setOption(*_set, "LeafCapacity", value);
}

unsigned int RTreeOptions::LeafPoolCapacity::get()
{
	return getOption(*_set, "LeafPoolCapacity", DEFAULT_LEAF_POOL_CAPACITY);
}

void RTreeOptions::LeafPoolCapacity::set(unsigned int value)
{
	setOption(*_set, "LeafPoolCapacity", value);
}

unsigned int RTreeOptions::RegionPoolCapacity::get()
{
	return getOption(*_set, "RegionPoolCapacity", DEFAULT_REGION_POOL_CAPACITY);
}

void RTreeOptions::RegionPoolCapacity::set(unsigned int value)
{
	setOption(*_set, "RegionPoolCapacity", value);
}

unsigned int RTreeOptions::PointPoolCapacity::get()
{
	return getOption(*_set, "PointPoolCapacity", DEFAULT_POINT_POOL_CAPACITY);
}

void RTreeOptions::PointPoolCapacity::set(unsigned int value)
{
	setOption(*_set, "PointPoolCapacity", value);
}

unsigned int RTreeOptions::Dimensions::get()
{
	return getOption(*_set, "Dimension", DEFAULT_DIMENSION);
}

void RTreeOptions::Dimensions::set(unsigned int value)
{
	setOption(*_set, "Dimension", value);
}

bool RTreeOptions::EnsureTightMBRs::get()
{
	return getOption(*_set, "EnsureTightMBRs", DEFAULT_TIGHT_MBRS);
}

void RTreeOptions::EnsureTightMBRs::set(bool value)
{
	setOption(*_set, "EnsureTightMBRs", value);
}

RTreeVariant RTreeOptions::TreeVariant::get()
{
	return (RTreeVariant)getOption(*_set, "TreeVariant", DEFAULT_TREE_VARIANT);
}

void RTreeOptions::TreeVariant::set(RTreeVariant value)
{
	setOption(*_set, "TreeVariant", (int)value);
}

Int64 RTreeOptions::IndexIdentifier::get()
{
	return getOption(*_set, "IndexIdentifier", (Int64)::SpatialIndex::StorageManager::NewPage);
}

void RTreeOptions::IndexIdentifier::set(Int64 value)
{
	setOption(*_set, "IndexIdentifier", value);
}

void RTreeOptions::setOption(PropertySet &props, const char *option, double value)
{
	Variant val;
	val.m_varType = VariantType::VT_DOUBLE;
	val.m_val.dblVal = value;
	props.setProperty(option, val);
}

void RTreeOptions::setOption(PropertySet &props, const char *option, unsigned int value)
{
	Variant val;
	val.m_varType = VariantType::VT_ULONG;
	val.m_val.ulVal = value;
	props.setProperty(option, val);
}

void RTreeOptions::setOption(PropertySet &props, const char *option, int value)
{
	Variant val;
	val.m_varType = VariantType::VT_LONG;
	val.m_val.lVal = value;
	props.setProperty(option, val);
}

void RTreeOptions::setOption(PropertySet &props, const char *option, Int64 value)
{
	Variant val;
	val.m_varType = VariantType::VT_LONGLONG;
	val.m_val.llVal = value;
	props.setProperty(option, val);
}

void RTreeOptions::setOption(PropertySet &props, const char *option, bool value)
{
	Variant val;
	val.m_varType = VariantType::VT_BOOL;
	val.m_val.blVal = value;
	props.setProperty(option, val);
}

bool RTreeOptions::getOption(PropertySet &props, const char *option, bool defaultValue)
{
	auto val = props.getProperty(option);
	if (val.m_varType != VT_BOOL)
	{
		return defaultValue;
	}
	return val.m_val.blVal;
}

unsigned int RTreeOptions::getOption(PropertySet &props, const char *option, unsigned int defaultValue)
{
	auto val = props.getProperty(option);
	if (val.m_varType != VT_ULONG)
	{
		return defaultValue;
	}
	return val.m_val.ulVal;
}

int RTreeOptions::getOption(PropertySet &props, const char *option, int defaultValue)
{
	auto val = props.getProperty(option);
	if (val.m_varType != VT_LONG)
	{
		return defaultValue;
	}
	return val.m_val.lVal;
}

double RTreeOptions::getOption(PropertySet &props, const char *option, double defaultValue)
{
	auto val = props.getProperty(option);
	if (val.m_varType != VT_DOUBLE)
	{
		return defaultValue;
	}
	return val.m_val.dblVal;
}

Int64 RTreeOptions::getOption(PropertySet &props, const char *option, Int64 defaultValue)
{
	auto val = props.getProperty(option);
	if (val.m_varType != VT_LONGLONG)
	{
		return defaultValue;
	}
	return val.m_val.llVal;
}

generic<typename TValue>
RTree<TValue>::RTree(RTreeOptions ^initOptions, IStorageManager ^manager)
	: ISpatialIndex(manager)
{
	_options = gcnew RTreeOptions();
	
	byte *copyTo;
	uint32_t len;
	initOptions->_set->storeToByteArray(&copyTo, len);
	try
	{
		_options->_set->loadFromByteArray(copyTo);
	}
	finally
	{
		delete copyTo;
	}
}

generic<typename TValue>
RTree<TValue>::!RTree()
{
	delete _options;
	_options = nullptr;
}

generic<typename TValue>
RTree<TValue>::~RTree()
{
	this->!RTree();
}

generic<typename TValue>
RTreeOptions ^RTree<TValue>::Options::get()
{
	getIndex()->getIndexProperties(*_options->_set);
	return _options;
}

generic<typename TValue>
::SpatialIndex::ISpatialIndex *RTree<TValue>::createRealIndex(::SpatialIndex::IStorageManager *manager)
{
	PropertySet *options = _options->_set;

	return ::SpatialIndex::RTree::returnRTree(*manager, *options);
}

generic<typename TValue>
int RTree<TValue>::dimensions()
{ 
	return _options->Dimensions;
}