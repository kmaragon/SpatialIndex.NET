#pragma once

#include "IStorageManager.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		struct _dsm_internal_data
		{
			bool overwrite;

			std::string filename;
			uint32_t pageSize;
			std::string dfsuffix;
			std::string ifSuffix;
		};

		/// <summary>The storage manager for storing trees on disk</summary>
		public ref class DiskStorageManager : public IStorageManager
		{
		public:
			/// <summary>Construct a DiskStorageManager</summary>
			/// <param name="filename">The base name for both the index and data files to use</param>
			DiskStorageManager(System::String ^filename);

			/// </summary>Destructor</summary>
			~DiskStorageManager();

			/// <summary>Whether or not the files should be overwritten if they already exist</summary>
			property bool Overwrite
			{
				bool get();
				void set(bool val);
			}

			/// <summary>The base filename for both the index and data files</summary>
			property System::String ^Filename
			{
				System::String ^get();
				void set(System::String ^val);
			}

			/// <summary>The internal file page size to use for index storage</summary>
			property System::UInt32 PageSize
			{
				System::UInt32 get();
				void set(System::UInt32 val);
			}

			/// <summary>The suffix to use with the base filename for the data file</summary>
			property System::String ^DatafileSuffix
			{
				System::String ^get();
				void set(System::String ^val);
			}

			/// <summary>The suffix to use with the base filename for the index file</summary>
			property System::String ^IndexfileSuffix
			{
				System::String ^get();
				void set(System::String ^val);
			}

		internal:
			virtual ::SpatialIndex::IStorageManager *createUnmanagedManager() override;

		private:
			void setProperty(::Tools::PropertySet &properties, const char *name, const ::Tools::Variant &value);
			void fillProperties(::Tools::PropertySet &properties);
			void setStringVariant(::Tools::PropertySet &properties, const char *prop, const std::string &value);
			void setBoolVariant(::Tools::PropertySet &properties, const char *prop, bool value);
			void setULongVariant(::Tools::PropertySet &properties, const char *prop, uint32_t value);

			_dsm_internal_data *_props;
			bool _initialized;
		};
	}
}