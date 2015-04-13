using System.IO;
using System.Linq;
using Konscious.SpatialIndex.Test.Helpers;
using Konscious.SpatialIndex.Test.Implementations;
using Xunit;

namespace Konscious.SpatialIndex.Test
{
    public class RTreeIntegrationTests
    {
        [Fact]
        public void RTreeCanBeRecoveredFromDisk()
        {
            var filename = Path.GetTempFileName();
            long indexId = 0;

            // create the new storage manager
            using (var storageManager = new DiskStorageManager(filename))
            {
                var options = new RTreeOptions()
                {
                    Dimensions = 2,
                    EnsureTightMBRs = true,
                    TreeVariant = RTreeVariant.RStar,
                    FillFactor = 0.9
                };

                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = true;
                storageManager.PageSize = 1024;

                using (var rtree = new RTree<byte[]>(options, storageManager))
                {
                    rtree.Add(new Circle(new Point(-8, 2), 4), DataHelpers.GenerateSomeBytes(0x123021));
                    rtree.Add(new Circle(new Point(0, 2), 4), DataHelpers.GenerateSomeBytes(0x723321));
                    rtree.Add(new Circle(new Point(8, 2), 4), DataHelpers.GenerateSomeBytes(0x325721));
                    rtree.Add(new Circle(new Point(-4, -4), 0), DataHelpers.GenerateSomeBytes(0x452910));
                    rtree.Add(new Circle(new Point(4, -4), 0), DataHelpers.GenerateSomeBytes(0x7290900));

                    // sanity check
                    Assert.Equal(5, rtree.Count());
                    indexId = rtree.Options.IndexIdentifier;
                }
            }

            // recycle the storage manager
            using (var storageManager = new DiskStorageManager(filename))
            {
                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = false;
                storageManager.PageSize = 1024;

                var options = new RTreeOptions(indexId)
                {
                    Dimensions = 2,
                    EnsureTightMBRs = true,
                    TreeVariant = RTreeVariant.RStar,
                    FillFactor = 0.9
                };

                using (var rtree = new RTree<byte[]>(options, storageManager))
                {
                    // make sure my objects are still there
                    Assert.Equal(5, rtree.Count());

                    // make sure they are still correct
                    var intersection = rtree.IntersectsWith(new Circle(new Point(4, -4), 4));
                    Assert.Equal(3, intersection.Count());
                }
            }
        }


        [Fact]
        public void RTreeCustomShapesCanBeRecycled()
        {
            var filename = Path.GetTempFileName();
            long indexId = 0;

            // create the new storage manager
            using (var storageManager = new DiskStorageManager(filename))
            {
                var options = new RTreeOptions()
                {
                    Dimensions = 2,
                    EnsureTightMBRs = true,
                    TreeVariant = RTreeVariant.RStar,
                    FillFactor = 0.9
                };

                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = true;
                storageManager.PageSize = 1024;

                using (var rtree = new RTree<byte[]>(options, storageManager))
                {
                    rtree.Add(new Circle(new Point(0, 0), 16), DataHelpers.GenerateSomeBytes(0x123021));

                    // verify that our circle is a circle
                    var circle = rtree.IntersectsWith(new Point(0, 0));
                    Assert.Equal(1, circle.Count());
                    Assert.Equal(0, rtree.IntersectsWith(new Point(15.99, 15.99)).Count());
                    indexId = rtree.Options.IndexIdentifier;
                }
            }

            // recycle the storage manager
            using (var storageManager = new DiskStorageManager(filename))
            {
                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = false;
                storageManager.PageSize = 1024;

                var options = new RTreeOptions(indexId)
                {
                    Dimensions = 2,
                    EnsureTightMBRs = true,
                    TreeVariant = RTreeVariant.RStar,
                    FillFactor = 0.9
                };

                using (var rtree = new RTree<byte[]>(options, storageManager))
                {
                    // verify that our circle is a circle
                    Assert.Equal(1, rtree.IntersectsWith(new Point(0, 0)).Count());
                    Assert.Equal(0, rtree.IntersectsWith(new Point(15.5, 15.5)).Count());
                }
            }
        }
    }
}
