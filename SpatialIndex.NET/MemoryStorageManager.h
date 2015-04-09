#pragma once

#include "IStorageManager.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>The storage manager for storing trees in native heap memory (rather than .NET managed heap memory)</summary>
		public ref class MemoryStorageManager : public IStorageManager
		{
		public:
			/// <summary>Construct a MemoryStorageManager</summary>
			MemoryStorageManager();

			/// </summary>Destructor</summary>
			~MemoryStorageManager();

		internal:
			virtual ::SpatialIndex::IStorageManager *createUnmanagedManager() override;
		};
	}
}