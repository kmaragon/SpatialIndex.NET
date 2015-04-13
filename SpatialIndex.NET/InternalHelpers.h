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
				
				static System::Collections::Generic::KeyValuePair<IShape ^, Object ^> pairFromData(const ::SpatialIndex::IData &in);

				generic<typename TValue>
				static cli::array<System::Byte> ^dataFromPair(IShape ^shape, TValue value);

				static IShape ^getManagedFromNative(const ::SpatialIndex::IShape *shape);
			};
		}
	}
}

