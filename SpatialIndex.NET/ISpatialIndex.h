#pragma once

#include "IShape.h"
#include "IStorageManager.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>The main interface for the library</summary>
		/// <remarks>
		/// An ISpatialIndex is a generic interface for any type of spatial index.
		/// Internally, this interface just wraps the interface by the same name 
		/// in libspatialindex
		///
		public ref class ISpatialIndex abstract : 
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^>>,
			System::IDisposable
		{
		public:
			/// <summary>Construct the index with the specified storage manager</summary>
			ISpatialIndex(IStorageManager ^manager);

			/// <summary>Destructor</summary>
			virtual ~ISpatialIndex();

			/// <summary>Add a shape and some data to the index</summary>
			void Add(IShape ^shape, cli::array<byte> ^data);

			/// <summary>Delete an Item from the index</summary>
			void Delete(IShape ^shape);

			/// <summary>Get an enumerable for traversing entries that are contained within a Shape</summary>
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^>> ^ContainsWhat(IShape ^Shape);

			/// <summary>Provide a callback for traversing entries that are contained within a Shape</summary>
			void ContainsWhat(IShape ^Shape, System::Action<IShape ^, array<byte> ^> ^callback);

			/// <summary>Get an enumerable for traversing entries that intersect a Shape</summary>
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^>> ^IntersectsWith(IShape ^Shape);
			
			/// <summary>Provide a callback for traversing entries that intersect a Shape</summary>
			void IntersectsWith(IShape ^Shape, System::Action<IShape ^, array<byte> ^> ^callback);

			/// <summary>Get the top n nearest neighbors to the Shape</summary>
			System::Collections::Generic::ICollection<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^>> ^NearestNeighbors(System::UInt32 count, IShape ^Shape);

			/// <summary>IEnumerable.GetEnumerator override</summary>
			/// <remarks>
			/// It is advisable to not use this. It loads the entire tree into a list and
			/// iterates on that. Otherwise, the only way to iterate through the items is through
			/// a native visitor pointer that doesn't nicely translate to an enumerator since the
			/// .NET state machine is not available in native code.
			/// </remarks>
			virtual System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^>> ^GetEnumerator();

			/// <summary>IEnumerable.GetEnumerator override</summary>
			/// <remarks>
			/// It is advisable to not use this. It loads the entire tree into a list and
			/// iterates on that. Otherwise, the only way to iterate through the items is through
			/// a native visitor pointer that doesn't nicely translate to an enumerator since the
			/// .NET state machine is not available in native code.
			/// </remarks>
			virtual System::Collections::IEnumerator ^GetObjectEnumerator() = System::Collections::IEnumerable::GetEnumerator;

			/// <summary>ToString() implementation</summary>
			System::String ^ToString() override;
			
		internal:
			virtual ::SpatialIndex::ISpatialIndex *createRealIndex(::SpatialIndex::IStorageManager *manager) = 0;
			virtual int dimensions() = 0;

		private:
			::SpatialIndex::ISpatialIndex *getIndex();

			IStorageManager ^_manager;
			::SpatialIndex::ISpatialIndex *_realIndex;
		};
	}
}

