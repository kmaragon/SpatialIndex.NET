#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>Kind of a base manager for both native and managed IStorageManager ports</summary>
		/// <remarks>
		/// Because of the C++/Nature of this class, it has to be an abstract class
		/// however, dear user, please think of it as an interface. The only thing this
		/// gives you is some memory management and native object wrapping
		/// </remarks>
		public ref class IStorageManager abstract : System::IDisposable
		{
		public:
			/// <summary>Constructor</summary>
			IStorageManager();

			/// <summary>Clean up the native stuff</summary>
			virtual ~IStorageManager();

		internal:
			/// <summary>create the native IStorageManager</summary>
			virtual ::SpatialIndex::IStorageManager *createUnmanagedManager() = 0;

			::SpatialIndex::IStorageManager *getManager();
		private:
			::SpatialIndex::IStorageManager *_unmanagedObject;
		};
	}
}
