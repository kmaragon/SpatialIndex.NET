using System.Linq;
using Konscious.SpatialIndex.Test.Helpers;
using Xunit;

namespace Konscious.SpatialIndex.Test
{
    [Trait("Type", "UnitTest")]
    public class MemoryStorageManagerTests
    {
        [Fact]
        public void MemoryStorageManager_CanStoreAndLoad()
        {
            var storageManager = new MemoryStorageManager();

            // a random but repeatable seed
            var buffer = DataHelpers.GenerateSomeBytes(9087449);
            var nAccess = new StorageManagerAccess(storageManager);
            var page1 = nAccess.Store(-1, buffer);

            var buffer2 = DataHelpers.GenerateSomeBytes(37900452);
            var page2 = nAccess.Store(-1, buffer2);

            Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
            Assert.True(buffer2.SequenceEqual(nAccess.Load(page2)));

            buffer = DataHelpers.GenerateSomeBytes(678000023);
            page1 = nAccess.Store(page1, buffer);
            Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
        }
    }
}
