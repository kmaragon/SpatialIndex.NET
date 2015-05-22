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
			delegate ISpatialIndexData ^CreateSpatialDataCallback(const ::SpatialIndex::IData &fromData);

			class QueryWrappedVisitor : public ::SpatialIndex::IVisitor
			{
			public:
				QueryWrappedVisitor(IShape ^query, CreateSpatialDataCallback ^dataCreator)
				{
					_query = query;
					_createData = dataCreator;
				}

				virtual ~QueryWrappedVisitor()
				{
					_query.release();
					delete _createData;
				}

				virtual void handle(ISpatialIndexData ^value) = 0;

			protected:
				virtual void visitNode(const ::SpatialIndex::INode &in);
				virtual void visitData(const ::SpatialIndex::IData &in);
				virtual void visitData(std::vector<const ::SpatialIndex::IData*> &v);

			private:
				msclr::auto_gcroot<IShape ^> _query;
				gcroot<CreateSpatialDataCallback ^> _createData;
			};

			void QueryWrappedVisitor::visitNode(const ::SpatialIndex::INode &in)
			{
				// we don't handle nodes
			}

			void QueryWrappedVisitor::visitData(const ::SpatialIndex::IData &in)
			{
				auto data = _createData->Invoke(in);
				this->handle(data);
			}

			void QueryWrappedVisitor::visitData(std::vector<const ::SpatialIndex::IData*> &v)
			{
				for (unsigned int i = 0; i < v.size(); ++i)
				{
					visitData(*v[i]);
				}
			}
		}
	}
}

