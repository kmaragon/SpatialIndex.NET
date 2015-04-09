#include "stdafx.h"
#include "ISpatialIndex.h"
#include "ListConstructionVisitor.h"
#include "ActionExecutorVisitor.h"
#include "WholeWorldShape.h"
#include "SpatialIndexException.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_shapes;
using namespace Konscious::SpatialIndex::_visitors;
using namespace System;
using namespace System::Collections::Generic;

ISpatialIndex::ISpatialIndex(IStorageManager ^manager)
{
	_realIndex = nullptr;
	_manager = manager;
}

ISpatialIndex::~ISpatialIndex()
{
	if (_realIndex != nullptr)
	{
		delete _realIndex;
		_realIndex = nullptr;
	}
}

void ISpatialIndex::Add(IShape ^shape, cli::array<byte> ^data)
{
	pin_ptr<byte> pinnedData = &data[0];
	try
	{
		getIndex()->insertData(data->Length, (byte*)pinnedData, *shape->getShape(), shape->Id);
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
}

void ISpatialIndex::Delete(IShape ^shape)
{
	try
	{
		getIndex()->deleteData(*shape->getShape(), shape->Id);
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
}

IEnumerable<KeyValuePair<IShape ^, array<byte> ^>> ^ISpatialIndex::ContainsWhat(IShape ^Shape)
{
	ListConstructionVisitor visitor;
	getIndex()->containsWhatQuery(*Shape->getShape(), visitor);
	return visitor.Data();
}

void ISpatialIndex::ContainsWhat(IShape ^Shape, Action<IShape ^, array<byte> ^> ^callback)
{
	ActionExecutorVisitor visitor(callback);
	getIndex()->containsWhatQuery(*Shape->getShape(), visitor);
}

IEnumerable<KeyValuePair<IShape ^, array<byte> ^>> ^ISpatialIndex::IntersectsWith(IShape ^Shape)
{
	ListConstructionVisitor visitor;
	getIndex()->intersectsWithQuery(*Shape->getShape(), visitor);
	return visitor.Data();
}

void ISpatialIndex::IntersectsWith(IShape ^Shape, System::Action<IShape ^, array<byte> ^> ^callback)
{
	ActionExecutorVisitor visitor(callback);
	getIndex()->intersectsWithQuery(*Shape->getShape(), visitor);
}

ICollection<KeyValuePair<IShape ^, array<byte> ^>> ^ISpatialIndex::NearestNeighbors(System::UInt32 count, IShape ^Shape)
{
	ListConstructionVisitor visitor;
	getIndex()->nearestNeighborQuery(count, *Shape->getShape(), visitor);
	return visitor.Data();
}

IEnumerator<KeyValuePair<IShape ^, array<byte> ^>> ^ISpatialIndex::GetEnumerator()
{
	ListConstructionVisitor visitor;
	WholeWorldShape scan(dimensions());
	getIndex()->intersectsWithQuery(scan, visitor);

	return visitor.Data()->GetEnumerator();
}

System::Collections::IEnumerator ^ISpatialIndex::GetObjectEnumerator()
{
	return this->GetEnumerator();
}

String ^ISpatialIndex::ToString()
{
	std::ostringstream wrstream;

	wrstream << *getIndex();
	wrstream.flush();

	return gcnew String(wrstream.str().c_str());
}

::SpatialIndex::ISpatialIndex *ISpatialIndex::getIndex()
{
	if (_realIndex == nullptr)
	{
		_realIndex = this->createRealIndex(_manager->getManager());
	}
	return _realIndex;
}
