using System;
using System.Linq;
using Konscious.SpatialIndex.Test.Implementations;
using SpatialIndex.NET.Test.Helpers;
using Xunit;

namespace Konscious.SpatialIndex.Test
{
    [Trait("Type", "IntegrationTest")]
    [Trait("IntegrationLevel", "Component")]
    public class ManagedStorageManagerTests
    {
        [Fact]
        public void ManagedStorageManager_CanStoreAndLoad()
        {
            // a random but repeatable seed
            var buffer = DataHelpers.GenerateSomeBytes(10025668);
            var nAccess = new StorageManagerAccess(new ManagedMemoryStorageManager());
            var page1 = nAccess.Store(-1, buffer);

            var buffer2 = DataHelpers.GenerateSomeBytes(10024938);
            var page2 = nAccess.Store(-1, buffer2);

            Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
            Assert.True(buffer2.SequenceEqual(nAccess.Load(page2)));

            buffer = DataHelpers.GenerateSomeBytes(10778910);
            page1 = nAccess.Store(page1, buffer);
            Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
        }

        [Fact]
        public void ManagedStorageManager_CanStoreAndDelete()
        {
            // a random but repeatable seed
            var buffer = DataHelpers.GenerateSomeBytes(213688);
            var nAccess = new StorageManagerAccess(new ManagedMemoryStorageManager());
            var page1 = nAccess.Store(-1, buffer);

            var buffer2 = DataHelpers.GenerateSomeBytes(9048991);
            var page2 = nAccess.Store(-1, buffer2);

            Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
            Assert.True(buffer2.SequenceEqual(nAccess.Load(page2)));

            nAccess.Delete(page2);
            Assert.Throws<IndexOutOfRangeException>(() => nAccess.Load(page2));
            Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));

            nAccess.Delete(page1);
            Assert.Throws<IndexOutOfRangeException>(() => nAccess.Load(page1));
            Assert.Throws<IndexOutOfRangeException>(() => nAccess.Load(page2));
        }

        [Fact]
        public void ManagedStorageManager_FlushSanityCheck()
        {
            // a random but repeatable seed
            var buffer = DataHelpers.GenerateSomeBytes(8980443);
            var nAccess = new StorageManagerAccess(new ManagedMemoryStorageManager());
            var page1 = nAccess.Store(-1, buffer);

            nAccess.Flush();
            nAccess.Delete(page1);
        }

    }
}
