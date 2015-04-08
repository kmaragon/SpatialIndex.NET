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
        public void TestRTreeInsertion()
        {
            var options = new RTreeOptions()
            {
                Dimensions = 2,
                EnsureTightMBRs = true,
                TreeVariant = RTreeVariant.RStar,
                FillFactor = 0.9
            };

            var rtree = new RTree(options, new ManagedMemoryStorageManager());
            var bytes = DataHelpers.GenerateSomeBytes(1024569);
            rtree.Add(new IShape(), bytes);
        }
    }
}
