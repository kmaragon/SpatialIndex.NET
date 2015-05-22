#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)
#include <msclr/auto_gcroot.h>
#include "IShape.h"
#include "QueryWrappedVisitor.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _visitors
		{
			class ListConstructionVisitor : public QueryWrappedVisitor
			{
			public:
				ListConstructionVisitor(IShape ^query)
					: QueryWrappedVisitor(query, gcnew CreateSpatialDataCallback(&CreateDataAsObject))
				{
					_list = gcnew System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> >();
				}

				~ListConstructionVisitor()
				{
					_list.release();
				}

				virtual void handle(ISpatialIndexData ^value);

				virtual System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>> ^Data();

			private:
				static ISpatialIndexData ^CreateDataAsObject(const ::SpatialIndex::IData &in)
				{
					return gcnew SpatialIndexData<System::Object ^>(in);
				}

				msclr::auto_gcroot<System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>> ^> _list;
			};

			void ListConstructionVisitor::handle(ISpatialIndexData ^value)
			{
				auto objData = (SpatialIndexData<System::Object ^> ^)value;
				_list.get()->Add(System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>(objData->Shape, objData->Value));
			}

			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>> ^ListConstructionVisitor::Data()
			{
				return _list.get();
			}
		}
	}
}

