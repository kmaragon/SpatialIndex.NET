#include "stdafx.h"
#include "IShape.h"

using namespace Konscious::SpatialIndex;
using namespace System;

IShape::IShape()
{
}

IShape::~IShape()
{
}

Int64 IShape::Id::get()
{
	return _id;
}

void IShape::Id::set(Int64 value)
{
	_id = value;
}