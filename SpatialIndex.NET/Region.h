#pragma once

#include "Point.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>A simple n-dimension region</summary>
		public ref class Region : IShape
		{
		public:
			Region(cli::array<double> ^min, cli::array<double> ^max);
			Region(Point ^min, Point ^max);

			!Region();
			virtual ~Region();

			property Point ^Minimum
			{
				Point ^get();
			}

			property Point ^Maximum
			{
				Point ^get();
			}

			virtual property Region ^BoundingRegion
			{
				Region ^get() override;
			}

		internal:
			Region(::SpatialIndex::Region *region);

			virtual ::SpatialIndex::IShape *getShape() override;
			virtual unsigned int dimensions() override;

			::SpatialIndex::Region *_region;
		};
	}
}

