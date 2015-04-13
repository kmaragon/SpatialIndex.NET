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
				ListConstructionVisitor(IShape ^query, FilterCallback ^filterCallback) 
					: QueryWrappedVisitor(query, filterCallback)
				{
					_list = gcnew System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^> >();
				}

				~ListConstructionVisitor()
				{
					_list.release();
				}

				virtual void handle(IShape ^key, System::Object ^value);

				virtual System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>> ^Data();

			private:
				msclr::auto_gcroot<System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>> ^> _list;
			};

			void ListConstructionVisitor::handle(IShape ^key, System::Object ^value)
			{
				_list.get()->Add(System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>(key, value));
			}

			System::Collections::Generic::IEnumerable<System::Collections::Generic::KeyValuePair<IShape ^, System::Object ^>> ^ListConstructionVisitor::Data()
			{
				return _list.get();
			}
		}
	}
}

