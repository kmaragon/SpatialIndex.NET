using System;
using System.Linq;
using System.Text;
using Konscious.SpatialIndex.Test.Helpers;
using Konscious.SpatialIndex.Test.Implementations;
using Xunit;

namespace Konscious.SpatialIndex.Test
{
    [Trait("Type", "UnitTest")]
    public class RTreeTests
    {
        [Fact]
        public void GetEnumerator_TestViaCount()
        {
            var rtree = SetupAnRTreeWithMy4Shapes();

            Assert.Equal(4, rtree.Count());
        }

        [Fact]
        public void Intersects_Test()
        {
            var rtree = SetupAnRTreeWithMy4Shapes();

            var matches = rtree.IntersectsWith(new Point(new[] {7.0, -0.5}));
            var byteCollection = matches.Select(node => Encoding.UTF8.GetString(node.Value)).ToList();

            Assert.Contains("Square1", byteCollection);
            Assert.Contains("Square2", byteCollection);
            Assert.Contains("Circle", byteCollection);
            Assert.DoesNotContain("Square3", byteCollection);
        }

        [Fact]
        public void ContainsWhat_Test()
        {
            var rtree = SetupAnRTreeWithMy4Shapes();

            var matches = rtree.ContainsWhat(new Region(new[] {2.9, -1.1}, new[] {11.1, 7.1}));
            var byteCollection = matches.Select(node => Encoding.UTF8.GetString(node.Value)).ToList();

            Assert.Contains("Circle", byteCollection);
            Assert.DoesNotContain("Square1", byteCollection);
            Assert.DoesNotContain("Square2", byteCollection);
            Assert.DoesNotContain("Square3", byteCollection);
        }

        [Fact]
        public void NearestNeighbor_TestWith1()
        {
            var rtree = SetupAnRTreeWithMy4Shapes();

            var matches = rtree.NearestNeighbors(1, new Point(new [] { 11.0, 7.0 }));
            var byteCollection = matches.Select(node => Encoding.UTF8.GetString(node.Value)).ToList();

            Assert.Contains("Circle", byteCollection);
            Assert.DoesNotContain("Square1", byteCollection);
            Assert.DoesNotContain("Square2", byteCollection);
            Assert.DoesNotContain("Square3", byteCollection);
        }

        [Fact]
        public void NearestNeighbor_TestWith2()
        {
            var rtree = SetupAnRTreeWithMy4Shapes();

            var matches = rtree.NearestNeighbors(2, new Point(new[] { 11.0, 7.0 }));
            var byteCollection = matches.Select(node => Encoding.UTF8.GetString(node.Value)).ToList();

            Assert.Contains("Circle", byteCollection);
            Assert.Contains("Square1", byteCollection);

            Assert.True(byteCollection.IndexOf("Circle") < byteCollection.IndexOf("Square1"));

            Assert.DoesNotContain("Square2", byteCollection);
            Assert.DoesNotContain("Square3", byteCollection);
        }

        [Fact]
        public void TestWithActualObject()
        {
            var options = new RTreeOptions()
            {
                Dimensions = 2,
                EnsureTightMBRs = true,
                TreeVariant = RTreeVariant.RStar,
                FillFactor = 0.9
            };

            var rtree = new RTree<RTreeTestValueType>(options, new ManagedMemoryStorageManager());

            var square1 = new Region(new[] { 5.0, -2.0 }, new[] { 12.0, 0.0 });
            var square2 = new Region(new[] { -2.0, -5.0 }, new[] { 7, 0.0 });
            var square3 = new Region(new[] { -5.0, 1.0 }, new[] { 0, 4.0 });
            var circle = new Circle(new Point(new[] { 7.0, 3.0 }), 4);

            var sq1data = new RTreeTestValueType()
            {
                Id = Guid.NewGuid(),
                Name = "Square1",
                Index = 0,
                Description = "The Description for square1's Object"
            };
            rtree.Add(square1, sq1data);

            var sq2data = new RTreeTestValueType()
            {
                Id = Guid.NewGuid(),
                Name = "Square2",
                Index = 1,
                Description = "The Description for square2's Object"
            };
            rtree.Add(square2, sq2data);

            var sq3data = new RTreeTestValueType()
            {
                Id = Guid.NewGuid(),
                Name = "Square3",
                Index = 2,
                Description = "The Description for square3's Object"
            };
            rtree.Add(square3, sq3data);

            var circledata = new RTreeTestValueType()
            {
                Id = Guid.NewGuid(),
                Name = "Circle",
                Index = 3,
                Description = "The Description for the only Circle Object"
            };
            rtree.Add(circle, circledata);

            var neighbors = rtree.NearestNeighbors(1, new Point(new[] { 11.0, 7.0 }));
            var neighborCollection = neighbors.Select(node => node.Value).ToList();

            Assert.Equal(1, neighborCollection.Count);
            Assert.Equal(3, neighborCollection[0].Index);


            var intersects = rtree.IntersectsWith(new Point(new[] { 7.0, -0.5 }));
            var intersectCollection = intersects.Select(node => node.Value.Index).ToList();

            Assert.Equal(3, intersectCollection.Count);
            Assert.Contains(0, intersectCollection);
            Assert.Contains(1, intersectCollection);
            Assert.Contains(3, intersectCollection);
            Assert.DoesNotContain(2, intersectCollection);

            var intersectNameCollection = intersects.Select(node => node.Value.Name).ToList();
            Assert.Contains("Square1", intersectNameCollection);
            Assert.Contains("Square2", intersectNameCollection);
            Assert.Contains("Circle", intersectNameCollection);
            Assert.DoesNotContain("Square3", intersectNameCollection);
        }

        private RTree<byte[]> SetupAnRTreeWithMy4Shapes()
        {
            var options = new RTreeOptions()
            {
                Dimensions = 2,
                EnsureTightMBRs = true,
                TreeVariant = RTreeVariant.RStar,
                FillFactor = 0.9
            };

            var rtree = new RTree<byte[]>(options, new ManagedMemoryStorageManager());
            var square1 = new Region(new[] { 5.0, -2.0 }, new[] { 12.0, 0.0 });
            var square2 = new Region(new[] { -2.0, -5.0 }, new[] { 7, 0.0 });
            var square3 = new Region(new[] { -5.0, 1.0 }, new[] { 0, 4.0 });
            var circle = new Circle(new Point(new[] { 7.0, 3.0 }), 4);

            var sq1bytes = Encoding.UTF8.GetBytes("Square1");
            rtree.Add(square1, sq1bytes);

            var sq2bytes = Encoding.UTF8.GetBytes("Square2");
            rtree.Add(square2, sq2bytes);

            var sq3bytes = Encoding.UTF8.GetBytes("Square3");
            rtree.Add(square3, sq3bytes);

            var circlebytes = Encoding.UTF8.GetBytes("Circle");
            rtree.Add(circle, circlebytes);

            return rtree;
        }
    }
}
