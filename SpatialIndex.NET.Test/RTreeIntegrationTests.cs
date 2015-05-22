using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
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
        public void DoABunchOfStuffFromDisk()
        {
            var filename = Path.GetTempFileName();
            long indexId = 0;

            // create the new storage manager
            using (var storageManager = new DiskStorageManager(filename))
            {
                storageManager.DatafileSuffix = "data";
                storageManager.IndexfileSuffix = "index";
                storageManager.Overwrite = true;
                storageManager.PageSize = 1024;
                
                using (var rtree = GetAFullTree(storageManager))
                {
                    Assert.Equal(10000, rtree.Count);

                    indexId = rtree.Options.IndexIdentifier;
                }
            }

            var stopWatch = new Stopwatch();

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
                    EnsureTightMBRs = false,
                    TreeVariant = RTreeVariant.RStar,
                    FillFactor = 0.9,
                    IndexCapacity = 10000,
                    SplitDistributionFactor = 0.5,
                    NearMinimumOverlapFactor = 2
                };

                var rand = new Random();
                using (var rtree = new RTree<byte[]>(options, storageManager))
                {
                    Assert.Equal(10000, rtree.Count);

                    stopWatch.Start();

                    long totalMatch = 0;
                    for (int i = 0; i < 10000; ++i)
                    {
                        var count = CountAnIntersection(rtree, rand);
                        Assert.True(count < 10000);
                        totalMatch += count;
                    }
                    stopWatch.Stop();

                    PrintPerformanceReport(stopWatch.ElapsedMilliseconds, 500, totalMatch);
                }
            }
        }

        [Fact]
        public void DoABunchOfStuffAsync()
        {
            var stopWatch = new Stopwatch();
            // create the new storage manager
            using (var storageManager = new MemoryStorageManager())
            {
                var rand = new Random();
                using (var rtree = GetAFullTree(storageManager))
                {
                    // sanity check
                    Assert.Equal(10000, rtree.Count);

                    stopWatch.Start();
                    long totalCount = 0;

                    var tasks = new Task<bool>[10];
                    for (var taskIndex = 0; taskIndex < 10; ++taskIndex)
                    {
                        tasks[taskIndex] = Task.Factory.StartNew(() => {
                            for (int i = 0; i < 1000; ++i)
                            {
                                var count = CountAnIntersection(rtree, rand);
                                Interlocked.Add(ref totalCount, count);
                                if (count > 10000)
                                {
                                    return false;
                                }
                            }
                            return true;
                        });
                    }

                    Task.WaitAll(tasks);
                    Assert.True(tasks.All(tsk => tsk.Result));
                    stopWatch.Stop();

                    PrintPerformanceReport(stopWatch.ElapsedMilliseconds, 500, totalCount);
                }
            }
        }

        [Fact]
        public void DoABunchOfStuff()
        {
            var stopWatch = new Stopwatch();
            // create the new storage manager
            using (var storageManager = new MemoryStorageManager())
            {
                var rand = new Random();
                using (var rtree = GetAFullTree(storageManager))
                {
                    // sanity check
                    Assert.Equal(10000, rtree.Count);

                    stopWatch.Start();

                    long totalMatch = 0;
                    for (int i = 0; i < 10000; ++i)
                    {
                        var count = CountAnIntersection(rtree, rand);
                        Assert.True(count < 10000);
                        totalMatch += count;
                    }
                    stopWatch.Stop();

                    PrintPerformanceReport(stopWatch.ElapsedMilliseconds, 500, totalMatch);
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

        private int CountAnIntersection(RTree<byte[]> rtree, Random rand)
        {
            var intersection = rtree.IntersectsWith(new Point(rand.NextDouble() * 5000, rand.NextDouble() * 5000));
            return intersection.Count();
        }

        private RTree<byte[]> GetAFullTree(IStorageManager storageManager)
        {
            var options = new RTreeOptions()
            {
                Dimensions = 2,
                EnsureTightMBRs = false,
                TreeVariant = RTreeVariant.RStar,
                FillFactor = 0.9,
                IndexCapacity = 10000,
                SplitDistributionFactor = 0.5,
                NearMinimumOverlapFactor = 2
            };

            var rand = new Random();
            var rtree = new RTree<byte[]>(options, storageManager);
            for (int i = 0; i < 10000; ++i)
            {
                if (rand.Next() % 2 != 0)
                {
                    var randinsert = new Circle(rand.NextDouble() * 5000, rand.NextDouble() * 5000, rand.NextDouble() * 40);
                    rtree.Add(randinsert, DataHelpers.GenerateSomeBytes(rand.Next()));
                }
                else
                {
                    var coordMin = new[] { rand.NextDouble() * 5000, rand.NextDouble() * 5000 };
                    var coordMax = new[] { coordMin[0] + (rand.NextDouble() * 80), coordMin[1] + (rand.NextDouble() * 80) };

                    var regioninsert = new Region(coordMin, coordMax);
                    rtree.Add(regioninsert, DataHelpers.GenerateSomeBytes(rand.Next()));
                }
            }
            return rtree;
        }

        private void PrintPerformanceReport(long millis, int count, long totalCount)
        {
            Trace.TraceInformation(
                "Took {0} to process / {1} Milliseconds per query / Average: {2} qps\nQueries found {3} results total / Average {4} per query",
                millis,
                millis / count,
                (count * 1.0 / millis) * 1000.0,
                totalCount,
                totalCount / count);
        }
    }
}
