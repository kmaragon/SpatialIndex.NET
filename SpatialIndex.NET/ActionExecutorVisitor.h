#pragma once

#pragma managed(push, off)
#include <spatialindex/SpatialIndex.h>
#pragma managed(pop)
#include <vcclr.h>
#include "IShape.h"
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
				ActionExecutorVisitor(IShape ^query, System::Action<IShape ^, System::Object ^> ^action, FilterCallback ^filterCallback)
					: QueryWrappedVisitor(query, filterCallback)
				{
					_action = action;
				}

				~ActionExecutorVisitor()
				{
					delete _action;
				}

				virtual void handle(IShape ^key, System::Object ^value);

			private:
				gcroot<System::Action<IShape ^, System::Object ^> ^> _action;
			};

			void ActionExecutorVisitor::handle(IShape ^key, System::Object ^value)
			{
				_action->Invoke(key, value);
			}
		}
	}
}
