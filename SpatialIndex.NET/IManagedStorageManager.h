// SpatialIndex.NET.h

#pragma once

using namespace System;

#include "IStorageManager.h"

namespace Konscious 
{
	namespace SpatialIndex 
	{
		/**
		 * <summary>The base storage manager interface for RTree storage</summary>
		 */
		public ref class IManagedStorageManager abstract : IStorageManager
		{
		public:
			/**
			 * <summary>Load some bytes out of the storage manager</summary>
			 */
			virtual array<Byte> ^Load(int64_t page) = 0;

			/**
			 * <summary>Store some bytes in the storage manager</summary>
			 * <return>The page the data was just stored in</summary>
			 */
			virtual int64_t Store(array<Byte> ^data) = 0;

			/**
			* <summary>Store some bytes in the storage manager</summary>
			* <return>The page the data was just stored in</summary>
			*/
			virtual int64_t Store(int64_t suggestedPage, array<Byte> ^data) = 0;

			/**
			 * <summary>Load some bytes out of the storage manager</summary>
			 */
			virtual void Delete(int64_t page) = 0;

			/**
			 * <summary>Flush the storage manager</summary>
			 */
			virtual void Flush() = 0;

		internal:
			virtual ::SpatialIndex::IStorageManager *createUnmanagedManager() override;
		};
	}
}
