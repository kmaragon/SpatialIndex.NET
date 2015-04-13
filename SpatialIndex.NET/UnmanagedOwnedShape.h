#pragma once

#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _shapes
		{
			private ref class UnmanagedOwnedShape : IShape
			{
			public:
				UnmanagedOwnedShape(const ::SpatialIndex::IShape *realShape);
				~UnmanagedOwnedShape();

				virtual property Region ^BoundingRegion
				{
					Region ^get() override;
				}

			internal:
				virtual ::SpatialIndex::IShape *getShape() override;
				virtual unsigned int dimensions() override;

			private:
				const ::SpatialIndex::IShape *_shape;
			};
		}
	}
}
