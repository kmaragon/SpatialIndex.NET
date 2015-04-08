#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _shapes
		{
			class WholeWorldShape : public ::SpatialIndex::IShape
			{
			public:
				WholeWorldShape(int dimensions);

				virtual ~WholeWorldShape();

				virtual bool intersectsShape(const::SpatialIndex::IShape& in) const;
				virtual bool containsShape(const ::SpatialIndex::IShape& in) const;
				virtual bool touchesShape(const ::SpatialIndex::IShape& in) const;
				virtual void getCenter(::SpatialIndex::Point& out) const;
				virtual uint32_t getDimension() const;
				virtual void getMBR(::SpatialIndex::Region& out) const;
				virtual double getArea() const;
				virtual double getMinimumDistance(const IShape& in) const;
				
				virtual uint32_t getByteArraySize();
				virtual void loadFromByteArray(const byte* data);
				virtual void storeToByteArray(byte** data, uint32_t& length);

			private:
				int _dimensions;
			};
		}
	}
}

