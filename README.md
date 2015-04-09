# SpatialIndex.NET
A library providing .NET bindings for libspatialindex

## Getting Started
1. Install the NuGet package
2. Add a reference to SpatialIndex.NET

### StorageManager
libspatialindex RTree's are powered by StorageManagers. To insantiate a tree, you'll need to also build a storage manager. Packaged with SpatialIndex.NET is DiskStorageManager and MemoryStorageManager (or there's about to be). You can build your own in pure .NET by deriving from IManagedStorageManager. 

This isn't technically an interface but the parts of it that are implemented are strictly the native code bindings. All of the .NET stuff is abstract. So it's sort of a psuedo interface. You'll see a bunch of those here.

### Options
The next thing you need to instantiate a tree is an RTreeOptions object. Details about each option can be found in the libspatialindex documentation.

### Shapes
Once the RTree is constructed, you can start putting shapes into it. Provided with the package are two basic shapes: Point and Region. In 2 dimensional space, a region is a rectangle. In 3 dimensions, it's a cube. A point is a point in space regardless of the dimensions. In many cases, you'll need more than just these two. This can be achieved by extending IManagedShapeBase to do all of the geometrical things that shapes should do.

### Exceptions
libspatialindex excecptions will be thrown in the form of SpatialIndexException. Any other weird native C++ exceptions bubble up as Exception with an error message.

## Building
Right now the linked libspatialindex is just here in libspatialindex. This is the pre-downloaded 64 bit windows binary. To build in 32 bit, you will currently need to change all of the config in the SpatialIndex.NET project to point to a 32 bit version. And of course the build configuration will change. If someone needs a 32 bit version - feel free to update the package to distribute both a 32 and 64 bit version. At the moment, I only need a 64 bit one.

The build is otherwise largely ready. You also might want an xunit test runner if you have a Visual Studio without support
