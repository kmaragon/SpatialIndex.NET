#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)

namespace Konscious
{
	namespace SpatialIndex
	{
		ref class Region;

		/// <summary>A pseudo-interface representing a geometric shape</summary>
		public ref class IShape abstract : System::IDisposable
		{
		public:
			IShape();
			virtual ~IShape();

			property System::Int64 Id
			{
				System::Int64 get();
				void set(System::Int64);
			}

			virtual property Region ^BoundingRegion
			{
				virtual Region ^get() = 0;
			}

		internal:
			virtual ::SpatialIndex::IShape *getShape() = 0;
			virtual unsigned int dimensions() = 0;

		private:
			int64_t _id;
		};
	}
}
