#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>A pseudo-interface representing a geometric shape</summary>
		public ref class IShape abstract : System::IDisposable
		{
		public:
			IShape();
			~IShape();

		internal:
			virtual ::SpatialIndex::IShape *createRealShape() = 0;

		private:
			::SpatialIndex::IShape *_realShape;
		};
	}
}
