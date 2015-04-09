using Konscious.SpatialIndex.Test.Implementations;
using Xunit;

namespace Konscious.SpatialIndex.Test.SelfTests
{
    public class CircleTests
    {
        [Fact]
        public void Intersects_TestBigGiantCover()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -20.0, -30.0 }), new Point(new[] { 20.0, 30.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestAllIsLeft()
        {
            var circle = new Circle(new Point(new[] {2.0, 2.0}), 4);
            var rectangle = new Region(new Point(new[] {-4.0, -3.0}), new Point(new[] {-3.0, 6.0}));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestLeftTouching()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -4.0, -3.0 }), new Point(new[] { -2.0, 6.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestLeftIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -4.0, -3.0 }), new Point(new[] { -1.0, 6.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestUpperLeftIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -4.0, 3.0 }), new Point(new[] { -1.0, 6.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestUpperLeftIsOutside()
        {
            var circle = new Circle(new Point(new[] { 0.0, 0.0 }), 2);
            var rectangle = new Region(new Point(new[] { -10.0, 1.99 }), new Point(new[] { -1.99, 10.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestAllIsUpper()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -8.0, 12.0 }), new Point(new[] { 10.0, 16.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestUpperTouching()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -8.0, 6.0 }), new Point(new[] { 10.0, 12.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestUpperIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -8.0, 5.0 }), new Point(new[] { 10.0, 12.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestUpperRightIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { 3.0, 5.0 }), new Point(new[] { 10.0, 12.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestUpperRightIsOutside()
        {
            var circle = new Circle(new Point(new[] { 0.0, 0.0 }), 2);
            var rectangle = new Region(new Point(new[] { 1.99, 1.99 }), new Point(new[] { 10.0, 10.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestAllIsRight()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { 8.0, -3.0 }), new Point(new[] { 12.0, 6.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestRightTouches()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { 6.0, -13.0 }), new Point(new[] { 12.0, 10.0 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestRightIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { 5.0, -13.0 }), new Point(new[] { 12.0, 10.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestBottomRightIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { 4.0, -11.0 }), new Point(new[] { 12.0, -1.0 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestBottomRightIsOutside()
        {
            var circle = new Circle(new Point(new[] { 0.0, 0.0 }), 2);
            var rectangle = new Region(new Point(new[] { 1.99, -10.0 }), new Point(new[] { 12.0, -1.99 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestAllIsBottom()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -13.0, -10.0 }), new Point(new[] { 12.0, -8.00 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestBottomTouches()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -13.0, -10.0 }), new Point(new[] { 12.0, -2.00 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestBottomIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -13.0, -10.0 }), new Point(new[] { 12.0, -1.00 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestBottomLeftIsInside()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -13.0, -10.0 }), new Point(new[] { 0.0, -1.00 }));

            Assert.True(circle.Intersects(rectangle));
        }

        [Fact]
        public void Intersects_TestBottomLeftIsOutside()
        {
            var circle = new Circle(new Point(new[] { 0.0, 0.0 }), 1);
            var rectangle = new Region(new Point(new[] { -13.0, -10.0 }), new Point(new[] { -1.99, -1.99 }));

            Assert.False(circle.Intersects(rectangle));
        }

        [Fact]
        public void Touches_TestLeftTouching()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -4.0, -3.0 }), new Point(new[] { -2.0, 6.0 }));

            Assert.True(circle.Touches(rectangle));
        }

        [Fact]
        public void Touches_TestUpperTouching()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -8.0, 6.0 }), new Point(new[] { 10.0, 12.0 }));

            Assert.True(circle.Touches(rectangle));
        }

        [Fact]
        public void Touches_TestRightTouches()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { 6.0, -3.0 }), new Point(new[] { 12.0, 6.0 }));

            Assert.True(circle.Touches(rectangle));
        }

        [Fact]
        public void Touches_TestBottomTouches()
        {
            var circle = new Circle(new Point(new[] { 2.0, 2.0 }), 4);
            var rectangle = new Region(new Point(new[] { -13.0, -10.0 }), new Point(new[] { 12.0, -2.00 }));

            Assert.True(circle.Touches(rectangle));
        }
    }
}
