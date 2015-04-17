#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)

#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		public interface class ISpatialIndexData : public System::IDisposable
		{
		};

		generic<typename TValueType>
		public ref class SpatialIndexData : public ISpatialIndexData
		{
		public:
			!SpatialIndexData();

			~SpatialIndexData();

			property IShape ^Shape
			{
				IShape ^get();
			}

			property TValueType Value
			{
				TValueType get();
			}

		internal:
			SpatialIndexData(const ::SpatialIndex::IData &dat);

		private:
			void checkInitialize();

			IShape ^_shape;
			TValueType _data;
			const ::SpatialIndex::IData &_idata;
		};
	}
}

