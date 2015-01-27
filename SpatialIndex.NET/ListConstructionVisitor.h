#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)
#include <msclr/auto_gcroot.h>
#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _visitors
		{
			class ListConstructionVisitor : public ::SpatialIndex::IVisitor
			{
			public:
				ListConstructionVisitor();
				~ListConstructionVisitor();

				System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^> > ^Data() const;

				virtual void visitNode(const ::SpatialIndex::INode &in);
				virtual void visitData(const ::SpatialIndex::IData &in);
				virtual void visitData(std::vector<const ::SpatialIndex::IData*> &v);

			private:
				msclr::auto_gcroot<System::Collections::Generic::List<System::Collections::Generic::KeyValuePair<IShape ^, array<byte> ^> > ^> _list;
			};
		}
	}
}

