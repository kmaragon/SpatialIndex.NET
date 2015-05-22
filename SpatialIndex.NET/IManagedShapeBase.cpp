#include "stdafx.h"
#include "IManagedShapeBase.h"

using namespace System;
using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_native;
using namespace System::Runtime::Serialization;

IManagedShapeBase::IManagedShapeBase()
{
	_wrapped = new ManagedShapeWrapper(this);
}

IManagedShapeBase::IManagedShapeBase(SerializationInfo ^info, StreamingContext context)
	: IManagedShapeBase()
{
	// nothing to deserialize in the abstract base
}

IManagedShapeBase::!IManagedShapeBase()
{
	delete _wrapped;
	_wrapped = nullptr;
}

IManagedShapeBase::~IManagedShapeBase()
{
	this->!IManagedShapeBase();
	GC::SuppressFinalize(this);
}

::SpatialIndex::IShape *IManagedShapeBase::getShape()
{
	return _wrapped;
}

unsigned int IManagedShapeBase::dimensions()
{
	return Dimensions;
}
