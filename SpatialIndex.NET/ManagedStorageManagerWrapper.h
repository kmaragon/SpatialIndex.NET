#pragma once

#include "IManagedStorageManager.h"
#include <msclr/auto_gcroot.h>

using namespace System::Runtime::InteropServices;

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _native
		{
			private class ManagedStorageManagerWrapper : public ::SpatialIndex::IStorageManager
			{
			public:
				ManagedStorageManagerWrapper(gcroot<IManagedStorageManager ^> realManager);

				virtual void loadByteArray(const ::SpatialIndex::id_type id, uint32_t &len, byte **data);

				virtual void storeByteArray(::SpatialIndex::id_type &id, const uint32_t len, const byte *const const_data);

				virtual void deleteByteArray(const ::SpatialIndex::id_type id);
				
				virtual void flush();

				~ManagedStorageManagerWrapper();

			private:
				msclr::auto_gcroot<IManagedStorageManager ^> managerHandle;
			};

		}
	}
}
