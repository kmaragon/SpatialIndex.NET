using System.Linq;
using System.Text;
using System.Xml.Schema;
using Konscious.SpatialIndex.Test.Helpers;
using Konscious.SpatialIndex.Test.Implementations;
using Xunit;

namespace Konscious.SpatialIndex.Test
{
    [Trait("Type", "IntegrationTest")]
    [Trait("IntegrationLevel", "Component")]
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
            Assert.DoesNotContain("Square2", byteCollection);
            Assert.DoesNotContain("Square3", byteCollection);
        }

        private RTree SetupAnRTreeWithMy4Shapes()
        {
            var options = new RTreeOptions()
            {
                Dimensions = 2,
                EnsureTightMBRs = true,
                TreeVariant = RTreeVariant.RStar,
                FillFactor = 0.9
            };

            var rtree = new RTree(options, new ManagedMemoryStorageManager());
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
