#pragma once

#using "SpatialIndex.NET.dll" as_friend

#include <spatialindex/SpatialIndex.h>

namespace Konscious 
{
	namespace SpatialIndex
	{
		namespace Test
		{
			public ref class StorageManagerAccess : System::IDisposable
			{
			public:
				StorageManagerAccess(IStorageManager ^manager);

				~StorageManagerAccess();

				array<System::Byte> ^Load(System::Int64 page);

				System::Int64 Store(System::Int64 page, array<System::Byte> ^data);

				void Delete(System::Int64 page);

				void Flush();

			private:
				::SpatialIndex::IStorageManager *_manager;
			};
		}
	}
}
