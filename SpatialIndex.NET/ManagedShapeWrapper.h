#pragma once

#include "IManagedShapeBase.h"
#include <msclr/auto_gcroot.h>

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _helpers
		{
			ref class InternalHelpers;
		}

		ref class IManagedShapeBase;

		namespace _native
		{
			class ManagedShapeWrapper : public ::SpatialIndex::IShape
			{
			public:
				ManagedShapeWrapper(gcroot<IManagedShapeBase ^> realShape);
				virtual ~ManagedShapeWrapper();

				virtual bool intersectsShape(const ::SpatialIndex::IShape& in) const;
				virtual bool containsShape(const ::SpatialIndex::IShape& in) const;
				virtual bool touchesShape(const ::SpatialIndex::IShape& in) const;
				virtual void getCenter(::SpatialIndex::Point& out) const;
				virtual uint32_t getDimension() const;
				virtual void getMBR(::SpatialIndex::Region& out) const;
				virtual double getArea() const;
				virtual double getMinimumDistance(const ::SpatialIndex::IShape& in) const;

				virtual uint32_t getByteArraySize();
				virtual void loadFromByteArray(const byte* data);
				virtual void storeToByteArray(byte** data, uint32_t& length);

			private:
				msclr::auto_gcroot<IManagedShapeBase ^> _shapeHandle;

				friend ref class _helpers::InternalHelpers;
			};
		}
	}
}