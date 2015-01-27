#pragma once

#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _shapes
		{
			private ref class UnmanagedBorrowedShape : IShape
			{
			public:
				UnmanagedBorrowedShape(::SpatialIndex::IShape *realShape);
				~UnmanagedBorrowedShape();

			internal:
				virtual ::SpatialIndex::IShape *getShape() override;

			private:
				::SpatialIndex::IShape *_shape;
			};
		}
	}
}
