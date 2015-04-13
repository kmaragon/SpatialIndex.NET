#include "stdafx.h"
#include "InternalHelpers.h"
#include "ISpatialIndex.h"
#include "ListConstructionVisitor.h"
#include "ActionExecutorVisitor.h"
#include "WholeWorldShape.h"
#include "SpatialIndexException.h"
#include "UnmanagedBorrowedShape.h"
#include "IManagedShapeBase.h"

using namespace Konscious::SpatialIndex;
using namespace Konscious::SpatialIndex::_shapes;
using namespace Konscious::SpatialIndex::_visitors;
using namespace Konscious::SpatialIndex::_helpers;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Linq;

generic<typename TValue>
ISpatialIndex<TValue>::ISpatialIndex(IStorageManager ^manager)
{
	_realIndex = nullptr;
	_manager = manager;
}

generic<typename TValue>
ISpatialIndex<TValue>::~ISpatialIndex()
{
	if (_realIndex != nullptr)
	{
		delete _realIndex;
		_realIndex = nullptr;
	}
}

generic<typename TValue>
void ISpatialIndex<TValue>::Add(IShape ^shape, TValue data)
{
	auto bytedata = InternalHelpers::dataFromPair(shape, data);
	pin_ptr<Byte> pinnedData = &bytedata[0];
	try
	{
		getIndex()->insertData(bytedata->Length, (byte*)pinnedData, *shape->getShape(), shape->Id);
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

generic<typename TValue>
void ISpatialIndex<TValue>::Delete(IShape ^shape)
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

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^, TValue> > ^ISpatialIndex<TValue>::ContainsWhat(IShape ^shape)
{
	try
	{
		ListConstructionVisitor visitor(shape, gcnew FilterCallback(&__contains));
		getIndex()->containsWhatQuery(*shape->getShape(), visitor);
		return Enumerable::Select<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(
			visitor.Data(), 
			gcnew Func<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(&__cast));
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

generic<typename TValue>
void ISpatialIndex<TValue>::ContainsWhat(IShape ^shape, Action<IShape ^, TValue> ^callback)
{
	try
	{
		auto wrapper = gcnew ActionWrapper(callback);

		ActionExecutorVisitor visitor(shape, gcnew Action<IShape ^, Object ^>(wrapper, &ActionWrapper::WrapCall), gcnew FilterCallback(&__contains));
		getIndex()->containsWhatQuery(*shape->getShape(), visitor);
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

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^, TValue> > ^ISpatialIndex<TValue>::IntersectsWith(IShape ^shape)
{
	try
	{
		ListConstructionVisitor visitor(shape, gcnew FilterCallback(&__intersects));
		getIndex()->intersectsWithQuery(*shape->getShape(), visitor);
		return Enumerable::Select<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(
			visitor.Data(),
			gcnew Func<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(&__cast));
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

generic<typename TValue>
void ISpatialIndex<TValue>::IntersectsWith(IShape ^shape, System::Action<IShape ^, TValue> ^callback)
{
	try
	{
		auto wrapper = gcnew ActionWrapper(callback);

		ActionExecutorVisitor visitor(shape, gcnew Action<IShape ^, Object ^>(wrapper, &ActionWrapper::WrapCall), gcnew FilterCallback(&__intersects));
		getIndex()->intersectsWithQuery(*shape->getShape(), visitor);
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

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^,TValue>> ^ISpatialIndex<TValue>::NearestNeighbors(System::UInt32 count, IShape ^shape)
{
	try
	{
		// this plus 1 sort thing isn't too reliable. There are lots of edge cases
		// that we don't get the desired behavior.
		ListConstructionVisitor visitor(shape, gcnew FilterCallback(&__nearest));
		getIndex()->nearestNeighborQuery(count + 1, *shape->getShape(), visitor);

		auto sorted = SortNeighbor(shape, (List<KeyValuePair<IShape ^, Object ^> > ^)visitor.Data());

		return Enumerable::Select(
			Enumerable::Take(sorted, count),
			gcnew Func<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(&__cast));
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

generic<typename TValue>
IEnumerator<KeyValuePair<IShape ^, TValue>> ^ISpatialIndex<TValue>::GetEnumerator()
{
	try
	{
		WholeWorldShape scan(dimensions());

		auto borrowed = gcnew UnmanagedBorrowedShape(&scan);
		// use __nearest because it arbitrarily returns true
		ListConstructionVisitor visitor(borrowed, gcnew FilterCallback(&__nearest));

		delete borrowed;
		getIndex()->intersectsWithQuery(scan, visitor);

		return Enumerable::Select<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(
			visitor.Data(),
			gcnew Func<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(&__cast))->GetEnumerator();
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

generic<typename TValue>
System::Collections::IEnumerator ^ISpatialIndex<TValue>::GetObjectEnumerator()
{
	return this->GetEnumerator();
}

generic<typename TValue>
String ^ISpatialIndex<TValue>::ToString()
{
	try
	{
		std::ostringstream wrstream;

		wrstream << *getIndex();
		wrstream.flush();

		return gcnew String(wrstream.str().c_str());
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

generic<typename TValue>
::SpatialIndex::ISpatialIndex *ISpatialIndex<TValue>::getIndex()
{
	if (_realIndex == nullptr)
	{
		_realIndex = this->createRealIndex(_manager->getManager());
	}
	return _realIndex;
}

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^, Object ^> > ^ISpatialIndex<TValue>::SortNeighbor(IShape ^query, List<KeyValuePair<IShape ^, Object ^> > ^list)
{
	auto comparer = gcnew DistanceComparer(query);
	list->Sort(comparer);
	return list;
}

generic<typename TValue>
bool ISpatialIndex<TValue>::__contains(IShape ^query, IShape ^queryShape)
{
	// region is best we'll do here
	return true;
}

generic<typename TValue>
bool ISpatialIndex<TValue>::__intersects(IShape ^query, IShape ^queryShape)
{
	// this can be bidirectional - let's make sure both ends work
	// spatialindex already took care of one end
	if (IManagedShapeBase::typeid->IsInstanceOfType(queryShape))
		return ((IManagedShapeBase ^)queryShape)->Intersects(query);

	return true;
}

generic<typename TValue>
bool ISpatialIndex<TValue>::__nearest(IShape ^query, IShape ^queryShape)
{
	return true;
}

generic<typename TValue>
KeyValuePair<IShape ^, TValue> ISpatialIndex<TValue>::__cast(KeyValuePair<IShape ^, Object ^> pair)
{
	return KeyValuePair<IShape ^, TValue>(pair.Key, (TValue)pair.Value);
}

generic<typename TValue>
ISpatialIndex<TValue>::ActionWrapper::ActionWrapper(System::Action<IShape ^, TValue> ^realAct)
{
	Action = realAct;
}

generic<typename TValue>
void ISpatialIndex<TValue>::ActionWrapper::WrapCall(IShape ^shape, Object ^value)
{
	Action->Invoke(shape, (TValue)value);
}

generic<typename TValue>
ISpatialIndex<TValue>::DistanceComparer::DistanceComparer(IShape ^query)
{
	_query = query;
}

generic<typename TValue>
int ISpatialIndex<TValue>::DistanceComparer::Compare(KeyValuePair<IShape ^, System::Object ^> a, KeyValuePair<IShape ^, System::Object ^> b)
{
	auto distA = a.Key->getShape()->getMinimumDistance(*_query->getShape());
	auto distB = b.Key->getShape()->getMinimumDistance(*_query->getShape());

	if (distA > distB)
		return 1;
	if (distA < distB)
		return -1;
	return 0;
}
