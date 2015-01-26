using System.IO;
using System.Linq;
using Konscious.SpatialIndex;
using Konscious.SpatialIndex.Test;
using SpatialIndex.NET.Test.Helpers;
using Xunit;

namespace SpatialIndex.NET.Test
{
    [Trait("Type", "IntegrationTest")]
    [Trait("IntegrationLevel", "Component")]
    public class DiskStorageManagerTests
    {
        [Fact]
        public void DiskStorageManager_CanStoreAndLoad()
        {
            var filename = Path.GetTempFileName();
            var storageManager = new DiskStorageManager(filename);

            storageManager.DatafileSuffix = "data";
            storageManager.IndexfileSuffix = "index";
            storageManager.Overwrite = true;
            storageManager.PageSize = 1024;

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

            Assert.True(File.Exists(filename + ".data"));
            Assert.True(File.Exists(filename + ".index"));
        }


        [Fact]
        public void DiskStorageManager_CanRestoreAndRead()
        {
            var filename = Path.GetTempFileName();
            long page1, page2, page3, dataFileSize, indexFileSize;
            byte[] buffer, buffer2, buffer3;

            // a random but repeatable seed
            buffer = DataHelpers.GenerateSomeBytes(165843578);
            using (var storageManager = new DiskStorageManager(filename))
            {
                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = true;
                storageManager.PageSize = 1024;

                using (var nAccess = new StorageManagerAccess(storageManager))
                {
                    page1 = nAccess.Store(-1, buffer);

                    buffer2 = DataHelpers.GenerateSomeBytes(46589725);
                    page2 = nAccess.Store(-1, buffer2);

                    Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
                    Assert.True(buffer2.SequenceEqual(nAccess.Load(page2)));

                    buffer = DataHelpers.GenerateSomeBytes(154967862);
                    page1 = nAccess.Store(page1, buffer);
                    Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
                }
            }

            // now that the files are closed get our fs view
            Assert.True(File.Exists(filename + ".data"));
            dataFileSize = new FileInfo(filename + ".data").Length;
            Assert.True(File.Exists(filename + ".index"));
            indexFileSize = new FileInfo(filename + ".index").Length;

            // ok now start again
            using (var storageManager = new DiskStorageManager(filename))
            {
                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = false;
                storageManager.PageSize = 1024;

                using (var nAccess = new StorageManagerAccess(storageManager))
                {

                    buffer3 = DataHelpers.GenerateSomeBytes(659873219);
                    page3 = nAccess.Store(-1, buffer3);

                    // make sure the files have not shrunk
                    Assert.True(new FileInfo(filename + ".data").Length >= dataFileSize);
                    Assert.True(new FileInfo(filename + ".index").Length >= indexFileSize);

                    // make sure all of our pages match
                    Assert.True(buffer.SequenceEqual(nAccess.Load(page1)));
                    Assert.True(buffer2.SequenceEqual(nAccess.Load(page2)));
                    Assert.True(buffer3.SequenceEqual(nAccess.Load(page3)));
                }
            }
        }
    }
}
