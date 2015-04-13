#pragma once

#include "ISpatialIndex.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		/// <summary>The R-Tree variant</summary>
		public enum class RTreeVariant
		{
			/// <summary>Linear</summary>
			Linear = ::SpatialIndex::RTree::RV_LINEAR,
			/// <summary>Quadratic</summary>
			Quadratic = ::SpatialIndex::RTree::RV_QUADRATIC,
			/// <summary>R-Star</summary>
			RStar = ::SpatialIndex::RTree::RV_RSTAR
		};

		/// <summary>A set of options to tweak R-Tree construction</summary>
		public ref class RTreeOptions
		{
		public:
			/// <summary>A constant value for IndexIdentifier to indicate that a new index should be created</summary>
			static const System::Int64 NewIndex = 0x8000000000000000L;

			/// <summary>Default Constructor</summary>
			RTreeOptions();

			/// <summary>Construct an RTree with an index identifier</summary>
			/// <remarks>
			/// StorageManagers can contain multiple Spatial Indexes. Each one has an identifier.
			/// This parameter specifies the index to use from the storage manager. If the manager
			/// already contains the tree with the index, it will be loaded from the existing slot
			/// in storage. If it's new, this will specify the identifier for getting the tree later
			/// </remarks>
			/// <param name="indexIdentifier">The id of the index that this RTree uses</param>
			RTreeOptions(System::Int64 indexIdentifier);

			/// <summary>Destructor</summary>
			~RTreeOptions();

			/// <summary>What ratio of pages should be filled with entries</summary>
			/// <remarks>
			/// A low value will have faster average insertions with a larger memory footprint
			/// A high value will be efficient on memory but will make insertions more likely
			/// to require new allocation pages and thus, slower.
			/// </remarks>
			property double FillFactor
			{
				double get();
				void set(double value);
			}

			property unsigned int NearMinimumOverlapFactor
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property double SplitDistributionFactor
			{
				double get();
				void set(double value);
			}

			property double ReinsertFactor
			{
				double get();
				void set(double value);
			}

			property unsigned int IndexCapacity
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property unsigned int IndexPoolCapacity
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property unsigned int LeafCapacity
			{
				unsigned int get();
				void set(unsigned int value);
			}
			
			property unsigned int LeafPoolCapacity
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property unsigned int RegionPoolCapacity
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property unsigned int PointPoolCapacity
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property unsigned int Dimensions
			{
				unsigned int get();
				void set(unsigned int value);
			}

			property bool EnsureTightMBRs
			{
				bool get();
				void set(bool value);
			}
			
			property RTreeVariant TreeVariant
			{
				RTreeVariant get();
				void set(RTreeVariant value);
			}

			property System::Int64 IndexIdentifier
			{
				System::Int64 get();
				void set(System::Int64 value);
			}

		private:
			static void setOption(::Tools::PropertySet &props, const char *option, double value);
			static void setOption(::Tools::PropertySet &props, const char *option, unsigned int value);
			static void setOption(::Tools::PropertySet &props, const char *option, int value);
			static void setOption(::Tools::PropertySet &props, const char *option, System::Int64 value);
			static void setOption(::Tools::PropertySet &props, const char *option, bool value);

			static bool getOption(::Tools::PropertySet &props, const char *option, bool defaultValue);
			static unsigned int getOption(::Tools::PropertySet &props, const char *option, unsigned int defaultValue);
			static int getOption(::Tools::PropertySet &props, const char *option, int defaultValue);
			static double getOption(::Tools::PropertySet &props, const char *option, double defaultValue);
			static System::Int64 getOption(::Tools::PropertySet &props, const char *option, System::Int64 defaultValue);

		internal:
			::Tools::PropertySet *_set;
		};

		/// <summary>RTree implementation of an ISpatialIndex</summary>
		generic<typename TValue>
		public ref class RTree : ISpatialIndex<TValue>
		{
		public:
			/// <summary>Construct an RTree</summary>
			/// <param name="initOption">The options with which to construct the tree</param>
			/// <param name="manager">The storage manager that will store the tree</param>
			RTree(RTreeOptions ^initOptions, IStorageManager ^manager);
			
			/// The options that are used in this RTree</summary>
			property RTreeOptions ^Options
			{
				RTreeOptions ^get();
			}

		internal:
			virtual ::SpatialIndex::ISpatialIndex *createRealIndex(::SpatialIndex::IStorageManager *manager) override;
			virtual int dimensions() override;

		private:
			RTreeOptions ^_options;
		};

	}
}
