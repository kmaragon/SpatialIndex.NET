#pragma once

#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>A simple point</summary>
		public ref class Point : IShape
		{
		public:
			Point(... cli::array<double> ^coords);

			!Point();
			~Point();

			property cli::array<double> ^Coordinates
			{
				cli::array<double> ^get();
			}

			property int Dimensions
			{
				int get();
			}

			virtual property Region ^BoundingRegion
			{
				Region ^get() override;
			}

		internal:
			Point(::SpatialIndex::Point *point);

			virtual ::SpatialIndex::IShape *getShape() override;
			virtual unsigned int dimensions() override;

			::SpatialIndex::Point *_point;
		};
	}
}
