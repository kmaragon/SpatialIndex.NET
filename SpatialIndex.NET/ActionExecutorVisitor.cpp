#include "stdafx.h"
#include "IShape.h"
#include "ActionExecutorVisitor.h"
#include "InternalHelpers.h"

using namespace cli;
using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_helpers;
using namespace Konscious::SpatialIndex::_visitors;
using namespace std;
using namespace System;
using namespace System::Collections::Generic;

ActionExecutorVisitor::ActionExecutorVisitor(Action<IShape ^, array<byte> ^> ^action)
{
	_action = action;
}

ActionExecutorVisitor::~ActionExecutorVisitor()
{
	delete _action;
}

void ActionExecutorVisitor::visitNode(const ::SpatialIndex::INode &in)
{
	// we don't handle nodes
}

void ActionExecutorVisitor::visitData(const ::SpatialIndex::IData &in)
{
	auto pair = InternalHelpers::pairFromData(in);
	_action->Invoke(pair.Key, pair.Value);
}

void ActionExecutorVisitor::visitData(vector<const ::SpatialIndex::IData*> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		visitData(*v[i]);
	}
}