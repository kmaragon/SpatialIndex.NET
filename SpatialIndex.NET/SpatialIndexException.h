#pragma once
#include <spatialindex/SpatialIndex.h>

namespace Konscious
{
	namespace SpatialIndex
	{
		[ System::SerializableAttribute ]
		public ref class SpatialIndexException : System::Exception
		{
		public:
			SpatialIndexException(::Tools::Exception &real);
		};
	}
}