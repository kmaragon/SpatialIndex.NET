#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)
#include <msclr/auto_gcroot.h>
#include "IShape.h"
#include "InternalHelpers.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _visitors
		{
			delegate bool FilterCallback(IShape ^query, IShape ^queryShape);

			class QueryWrappedVisitor : public ::SpatialIndex::IVisitor
			{
			public:
				QueryWrappedVisitor(IShape ^query, FilterCallback ^filterCallback)
				{
					_query = query;
					_filterCallback = filterCallback;
				}

				virtual ~QueryWrappedVisitor()
				{
					_query.release();
					delete _filterCallback;
				}

				virtual void handle(IShape ^key, System::Object ^value) = 0;

			protected:
				virtual void visitNode(const ::SpatialIndex::INode &in);
				virtual void visitData(const ::SpatialIndex::IData &in);
				virtual void visitData(std::vector<const ::SpatialIndex::IData*> &v);

			private:
				msclr::auto_gcroot<IShape ^> _query;
				gcroot<FilterCallback ^> _filterCallback;
			};

			void QueryWrappedVisitor::visitNode(const ::SpatialIndex::INode &in)
			{
				// we don't handle nodes
			}

			void QueryWrappedVisitor::visitData(const ::SpatialIndex::IData &in)
			{
				auto kvp = _helpers::InternalHelpers::pairFromData(in);
				if (_filterCallback->Invoke(_query.get(), kvp.Key))
					this->handle(kvp.Key, kvp.Value);
			}

			void QueryWrappedVisitor::visitData(std::vector<const ::SpatialIndex::IData*> &v)
			{
				for (int i = 0; i < v.size(); ++i)
				{
					visitData(*v[i]);
				}
			}
		}
	}
}

