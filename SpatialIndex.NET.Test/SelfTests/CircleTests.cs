using Konscious.SpatialIndex.Test.Implementations;
using Xunit;

namespace Konscious.SpatialIndex.Test.SelfTests
{
    public class CircleTests
    {
        [Fact]
        public void Intersects_TestAllIsLeft()
        {
            var circle = new Circle(new Point(new[] {-2.0, 2.0}), 4);
            var rectangle = new Region(new Point(new[] {-4.0, -3.0}), new Point(new[] {-3.0, 6.0}));

            Assert.False(circle.Intersects(rectangle));
        }
    }
}
