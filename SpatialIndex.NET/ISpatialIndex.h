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
		generic<typename TValue>
		public ref class ISpatialIndex abstract : 
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, TValue >>,
			System::IDisposable
		{
		public:
			/// <summary>Construct the index with the specified storage manager</summary>
			ISpatialIndex(IStorageManager ^manager);

			/// <summary>Disposer</summary>
			!ISpatialIndex();

			/// <summary>Destructor</summary>
			virtual ~ISpatialIndex();

			/// <summary>Add a shape and a value to the index</summary>
			void Add(IShape ^shape, TValue data);

			/// <summary>Delete an Item from the index</summary>
			void Delete(IShape ^shape);

			/// <summary>Get an enumerable for traversing entries that are contained within a Shape</summary>
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, TValue >> ^ContainsWhat(IShape ^Shape);

			/// <summary>Provide a callback for traversing entries that are contained within a Shape</summary>
			void ContainsWhat(IShape ^Shape, System::Action<IShape ^, TValue > ^callback);

			/// <summary>Get an enumerable for traversing entries that intersect a Shape</summary>
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, TValue >> ^IntersectsWith(IShape ^Shape);
			
			/// <summary>Provide a callback for traversing entries that intersect a Shape</summary>
			void IntersectsWith(IShape ^Shape, System::Action<IShape ^, TValue > ^callback);

			/// <summary>Get the top n nearest neighbors to the Shape</summary>
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, TValue >> ^NearestNeighbors(System::UInt32 count, IShape ^Shape);

			/// <summary>IEnumerable.GetEnumerator override</summary>
			/// <remarks>
			/// It is advisable to not use this. It loads the entire tree into a list and
			/// iterates on that. Otherwise, the only way to iterate through the items is through
			/// a native visitor pointer that doesn't nicely translate to an enumerator since the
			/// .NET state machine is not available in native code.
			/// </remarks>
			virtual System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<IShape ^, TValue >> ^GetEnumerator();

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
			::SpatialIndex::ISpatialIndex *getIndex();

		private:
			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> > ^SortNeighbor(
				IShape ^query,
				System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> > ^list
				);

			static bool __contains(IShape ^query, IShape ^queryShape);
			static bool __intersects(IShape ^query, IShape ^queryShape);
			static bool __nearest(IShape ^query, IShape ^queryShape);

			static System::Collections::Generic::KeyValuePair<IShape ^, TValue> __cast(System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> pair);
			
			ref struct ActionWrapper
			{
			public:
				ActionWrapper(System::Action<IShape ^, TValue> ^realAct);
				void WrapCall(IShape ^shape, System::Object ^value);
				System::Action<IShape ^, TValue> ^Action;
			};

			ref class DistanceComparer : public System::Collections::Generic::IComparer<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>>
			{
			public:
				DistanceComparer(IShape ^query);

				virtual int Compare(System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> a, System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> b);

			private:
				IShape ^_query;
			};

			IStorageManager ^_manager;
			::SpatialIndex::ISpatialIndex *_realIndex;
		};
	}
}

