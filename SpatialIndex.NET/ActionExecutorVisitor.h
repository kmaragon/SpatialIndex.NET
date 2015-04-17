#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)
#include <vcclr.h>
#include "IShape.h"
#include "SpatialIndexData.h"
#include "QueryWrappedVisitor.h"

namespace Konscious
{
	namespace SpatialIndex
	{
		namespace _visitors
		{
			class ActionExecutorVisitor : public QueryWrappedVisitor
			{
			public:
				ActionExecutorVisitor(IShape ^query, System::Action<ISpatialIndexData ^> ^action, CreateSpatialDataCallback ^createDataCallback)
					: QueryWrappedVisitor(query, createDataCallback)
				{
					_action = action;
				}

				~ActionExecutorVisitor()
				{
					delete _action;
				}

				virtual void handle(ISpatialIndexData ^value);

			private:
				gcroot<System::Action<ISpatialIndexData ^> ^> _action;
			};

			void ActionExecutorVisitor::handle(ISpatialIndexData ^value)
			{
				_action->Invoke(value);
			}
		}
	}
}
