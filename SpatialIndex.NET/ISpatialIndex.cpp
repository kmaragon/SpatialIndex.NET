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
using namespace System::Threading;

generic<typename TValue>
ISpatialIndex<TValue>::ISpatialIndex(IStorageManager ^manager)
{
	_realIndex = nullptr;
	_manager = manager;
}

generic<typename TValue>
ISpatialIndex<TValue>::!ISpatialIndex()
{
	if (_realIndex != nullptr)
	{
		Monitor::Enter(this);
		try
		{
			delete _realIndex;
			_realIndex = nullptr;
		}
		finally
		{
			Monitor::Exit(this);
		}
	}
}

generic<typename TValue>
ISpatialIndex<TValue>::~ISpatialIndex()
{
	this->!ISpatialIndex();
}

generic<typename TValue>
void ISpatialIndex<TValue>::Add(IShape ^shape, TValue data)
{
	auto bytedata = InternalHelpers::dataFromPair(shape, data);
	pin_ptr<Byte> pinnedData = &bytedata[0];

	// libspatial index isn't thread safe
	Monitor::Enter(this);

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
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
void ISpatialIndex<TValue>::Delete(IShape ^shape)
{
	// libspatial index isn't thread safe
	Monitor::Enter(this);

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
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
int ISpatialIndex<TValue>::Count::get()
{
	CountVisitor cv;

	WholeWorldShape scan(dimensions());

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		getIndex()->intersectsWithQuery(scan, cv);
		return cv.getResultCount();
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^, TValue> > ^ISpatialIndex<TValue>::ContainsWhat(IShape ^shape)
{
	ListConstructionVisitor visitor(shape);

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
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
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
void ISpatialIndex<TValue>::ContainsWhat(IShape ^shape, Action<SpatialIndexData<TValue> ^> ^callback)
{
	auto wrapper = gcnew ActionWrapper(callback);

	ActionExecutorVisitor visitor(shape, gcnew Action<ISpatialIndexData ^>(wrapper, &ActionWrapper::WrapCall), gcnew CreateSpatialDataCallback(&__createData));

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		// libspatial index isn't thread safe even for reads due to recycled memory buffers
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
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^, TValue> > ^ISpatialIndex<TValue>::IntersectsWith(IShape ^shape)
{
	auto checker = gcnew IntersectsChecker(shape);
	auto checkerDelegate = gcnew Func<KeyValuePair<IShape ^, TValue>, bool>(checker, &IntersectsChecker::Intersects);

	ListConstructionVisitor visitor(shape);

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		if (shape->GetType() == Point::typeid)
		{
			// special query for points
			getIndex()->pointLocationQuery(*((Point ^)shape)->_point, visitor);
		}
		else
		{
			getIndex()->intersectsWithQuery(*shape->getShape(), visitor);
		}
		auto unfiltered = Enumerable::Select<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(
			visitor.Data(),
			gcnew Func<KeyValuePair<IShape ^, Object ^>, KeyValuePair<IShape ^, TValue> >(&__cast));

		return Enumerable::Where<KeyValuePair<IShape ^, TValue>>(unfiltered, checkerDelegate);
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
void ISpatialIndex<TValue>::IntersectsWith(IShape ^shape, System::Action<SpatialIndexData<TValue> ^> ^callback)
{
	auto wrapper = gcnew ActionWrapper(callback);

	ActionExecutorVisitor visitor(shape, gcnew Action<ISpatialIndexData ^>(wrapper, &ActionWrapper::WrapCall), gcnew CreateSpatialDataCallback(&__createData));

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		if (shape->GetType() == Point::typeid)
		{
			getIndex()->pointLocationQuery(*((Point ^)shape)->_point, visitor);
		}
		else
		{
			getIndex()->intersectsWithQuery(*shape->getShape(), visitor);
		}
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
IEnumerable<KeyValuePair<IShape ^,TValue>> ^ISpatialIndex<TValue>::NearestNeighbors(System::UInt32 count, IShape ^shape)
{
	ListConstructionVisitor visitor(shape);

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		getIndex()->nearestNeighborQuery(count, *shape->getShape(), visitor);

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
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
void ISpatialIndex<TValue>::NearestNeighbors(System::UInt32 count, IShape ^shape, System::Action<SpatialIndexData<TValue> ^> ^callback)
{
	auto wrapper = gcnew ActionWrapper(callback);

	ActionExecutorVisitor visitor(shape, gcnew Action<ISpatialIndexData ^>(wrapper, &ActionWrapper::WrapCall), gcnew CreateSpatialDataCallback(&__createData));

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		getIndex()->nearestNeighborQuery(count, *shape->getShape(), visitor);
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
IEnumerator<KeyValuePair<IShape ^, TValue>> ^ISpatialIndex<TValue>::GetEnumerator()
{
	WholeWorldShape scan(dimensions());

	auto borrowed = gcnew UnmanagedBorrowedShape(&scan);
	ListConstructionVisitor visitor(borrowed);

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		getIndex()->intersectsWithQuery(scan, visitor);
		delete borrowed;

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
	finally
	{
		Monitor::Exit(this);
	}
}

generic<typename TValue>
System::Collections::IEnumerator ^ISpatialIndex<TValue>::GetObjectEnumerator()
{
	return this->GetEnumerator();
}

generic<typename TValue>
void ISpatialIndex<TValue>::ForEach(Action<SpatialIndexData<TValue> ^> ^callback)
{
	WholeWorldShape scan(dimensions());

	auto borrowed = gcnew UnmanagedBorrowedShape(&scan);
	auto wrapper = gcnew ActionWrapper(callback);
	ActionExecutorVisitor visitor(borrowed, gcnew Action<ISpatialIndexData ^>(wrapper, &ActionWrapper::WrapCall), gcnew CreateSpatialDataCallback(&__createData));

	// libspatial index isn't thread safe
	Monitor::Enter(this);

	try
	{
		getIndex()->intersectsWithQuery(scan, visitor);
		delete borrowed;
	}
	catch (::Tools::Exception &spatialException)
	{
		throw gcnew SpatialIndexException(spatialException);
	}
	catch (const std::exception &exc)
	{
		throw gcnew System::Exception(gcnew System::String(exc.what()));
	}
	finally
	{
		Monitor::Exit(this);
	}
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
ISpatialIndex<TValue>::IntersectsChecker::IntersectsChecker(IShape ^query)
{
	_query = query;
}

generic<typename TValue>
bool ISpatialIndex<TValue>::IntersectsChecker::Intersects(KeyValuePair<IShape ^, TValue> value)
{
	// this can be bidirectional - let's make sure both ends work
	// spatialindex already took care of one end
	if (IManagedShapeBase::typeid->IsInstanceOfType(value.Key))
		return ((IManagedShapeBase ^)value.Key)->Intersects(_query);

	return true;
}

generic<typename TValue>
ISpatialIndexData ^ISpatialIndex<TValue>::__createData(const ::SpatialIndex::IData &in)
{
	return gcnew SpatialIndexData<TValue>(in);
}

generic<typename TValue>
KeyValuePair<IShape ^, TValue> ISpatialIndex<TValue>::__cast(KeyValuePair<IShape ^, Object ^> pair)
{
	return KeyValuePair<IShape ^, TValue>(pair.Key, (TValue)pair.Value);
}

generic<typename TValue>
ISpatialIndex<TValue>::ActionWrapper::ActionWrapper(System::Action<SpatialIndexData<TValue> ^> ^realAct)
{
	Action = realAct;
}

generic<typename TValue>
void ISpatialIndex<TValue>::ActionWrapper::WrapCall(ISpatialIndexData ^value)
{
	Action->Invoke((SpatialIndexData<TValue> ^) value);
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
