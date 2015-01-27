#include "stdafx.h"

#include "IShape.h"
#include "ListConstructionVisitor.h"
#include "InternalHelpers.h"

using namespace cli;
using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_helpers;
using namespace Konscious::SpatialIndex::_visitors;
using namespace std;
using namespace System;
using namespace System::Collections::Generic;

ListConstructionVisitor::ListConstructionVisitor()
{
	_list = gcnew List<KeyValuePair<IShape ^, array<byte> ^> >();
}

List<KeyValuePair<IShape ^, array<byte> ^> > ^ListConstructionVisitor::Data() const
{
	return _list.get();
}

void ListConstructionVisitor::visitNode(const ::SpatialIndex::INode &in)
{
	// we don't handle nodes
}

void ListConstructionVisitor::visitData(const ::SpatialIndex::IData &in)
{
	_list->Add(InternalHelpers::pairFromData(in));
}

void ListConstructionVisitor::visitData(vector<const ::SpatialIndex::IData*> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		visitData(*v[i]);
	}
}

ListConstructionVisitor::~ListConstructionVisitor()
{
}
