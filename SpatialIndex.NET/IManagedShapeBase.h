#pragma once

#include "Region.h"
#include "ManagedShapeWrapper.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _native
		{
			class ManagedShapeWrapper;
		}

		public ref class IManagedShapeBase abstract : IShape, System::Runtime::Serialization::ISerializable
		{
		public:
			IManagedShapeBase();

			IManagedShapeBase(System::Runtime::Serialization::SerializationInfo ^info, System::Runtime::Serialization::StreamingContext context);

			!IManagedShapeBase();

			virtual ~IManagedShapeBase();

			virtual bool Intersects(IShape ^in) = 0;
			
			virtual bool Contains(IShape ^in) = 0;
			
			virtual bool Touches(IShape ^in) = 0;

			virtual double MinimumDistanceFrom(IShape ^in) = 0;

			virtual property Point ^Center
			{
				virtual Point ^get() = 0;
			}
			
			virtual property System::UInt32 Dimensions
			{
				virtual System::UInt32 get() = 0;
			}

			virtual property System::Double Area
			{
				virtual System::Double get() = 0;
			}

			[System::Security::Permissions::SecurityPermission(System::Security::Permissions::SecurityAction::LinkDemand, Flags = System::Security::Permissions::SecurityPermissionFlag::SerializationFormatter)]
			virtual void GetObjectData(System::Runtime::Serialization::SerializationInfo ^info, System::Runtime::Serialization::StreamingContext context) = 0;
			
		internal:
			virtual ::SpatialIndex::IShape *getShape() override;
			virtual unsigned int dimensions() override;

		private:
			Konscious::SpatialIndex::_native::ManagedShapeWrapper *_wrapped;
		};
	}
}
