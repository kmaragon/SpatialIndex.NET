#pragma once

#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _helpers
		{
			private ref class InternalHelpers
			{
			public:
				InternalHelpers();

				static System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^> pairFromData(const ::SpatialIndex::IData &in);

				static IShape ^getManagedFromNative(::SpatialIndex::IShape *shape);
			};
		}
	}
}

