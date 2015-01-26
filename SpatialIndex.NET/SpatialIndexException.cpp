#include "stdafx.h"
#include "SpatialIndexException.h"

using namespace Konscious::SpatialIndex;

SpatialIndexException::SpatialIndexException(::Tools::Exception &real) 
	: System::Exception(gcnew System::String(real.what().c_str()))
{
}
