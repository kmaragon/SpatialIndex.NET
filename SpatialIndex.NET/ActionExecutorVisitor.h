#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)
#include <vcclr.h>
#include "IShape.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _visitors
		{
			class ActionExecutorVisitor : public ::SpatialIndex::IVisitor
			{
			public:
				ActionExecutorVisitor(System::Action<IShape ^, array<byte> ^> ^action);
				~ActionExecutorVisitor();

				virtual void visitNode(const ::SpatialIndex::INode &in);
				virtual void visitData(const ::SpatialIndex::IData &in);
				virtual void visitData(std::vector<const ::SpatialIndex::IData*> &v);

			private:
				gcroot<System::Action<IShape ^, cli::array<byte> ^> ^> _action;
			};
		}
	}
}
