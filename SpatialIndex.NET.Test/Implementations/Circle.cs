using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Konscious.SpatialIndex.Test.Implementations
{
    public class Circle : IManagedShapeBase
    {
        public Circle(Point center, double radius)
        {
            if (center.Dimensions != 2)
            {
                throw new ArgumentException("Circle only supports 2 dimensions", "center");
            }

            if (radius < 0)
            {
                throw new ArgumentException("Radius must be positive", "radius");
            }

            _center = center;
            _radius = radius;
            _radiusSquared = radius*radius;
        }

        public override double Area
        {
            get { return Math.PI * _radiusSquared; }
        }

        public override Region BoundingRegion
        {
            get
            {
                var tl = new[] { _center.Coordinates[0] - _radius, _center.Coordinates[1] - _radius };
                var br = new[] { _center.Coordinates[0] + _radius, _center.Coordinates[1] + _radius };
                return new Region(tl, br);
            }
        }

        public override Point Center
        {
            get { return _center; }
        }

        public override bool Contains(IShape @in)
        {
            var squares = GetSquares(@in);

            // a^2 + b^2 > c^2
            // top right
            if ((squares.LeftSquared + squares.TopSquared) > _radiusSquared)
            {
                return false;
            }

            // bottom left
            if ((squares.LeftSquared + squares.BottomSquared) > _radiusSquared)
            {
                return false;
            }

            // top right
            if ((squares.RightSquared + squares.TopSquared) > _radiusSquared)
            {
                return false;
            }

            // bottom right
            if ((squares.RightSquared + squares.BottomSquared) > _radiusSquared)
            {
                return false;
            }

            return true;
        }

        public override uint Dimensions
        {
            get { return 2; }
        }

        public override void GetObjectData(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context)
        {
            info.AddValue("Radius", _radius);
            info.AddValue("CenterX", _center.Coordinates[0]);
            info.AddValue("CenterY", _center.Coordinates[1]);
        }

        public override bool Intersects(IShape @in)
        {
            var box = @in.BoundingRegion;

            // is the box just not crossing between the min and max boundaries of the circle?
            if (box.Maximum.Coordinates[0] <= (_center.Coordinates[0] - _radius))
                return false;
            if (box.Minimum.Coordinates[0] >= (_center.Coordinates[0] + _radius))
                return false;
            
            if (box.Maximum.Coordinates[1] <= (_center.Coordinates[1] - _radius))
                return false;
            if (box.Minimum.Coordinates[1] >= (_center.Coordinates[1] + _radius))
                return false;

            if (box.Minimum.Coordinates[0] <= _center.Coordinates[0])
            {
                // check if the rect x crosses over the peak of the circle
                if (box.Maximum.Coordinates[0] >= _center.Coordinates[0])
                    return true;

                if (box.Minimum.Coordinates[1] <= _center.Coordinates[1])
                {
                    // check if rect y crosses over the circle's peak
                    if (box.Maximum.Coordinates[1] >= _center.Coordinates[1])
                        return true;

                    // the rect is in circle's first local quadrant
                    var squares = GetSquares(@in);

                    // bottom right
                    return ((squares.RightSquared + squares.BottomSquared) < _radiusSquared);
                }
                else
                {
                    // the rect is in circle's third local quadrant
                    var squares = GetSquares(@in);

                    // top right
                    return ((squares.RightSquared + squares.TopSquared) < _radiusSquared);
                }

            }

            if (box.Minimum.Coordinates[1] <= _center.Coordinates[1])
            {
                // still if we vertically cross the center, we're good
                if (box.Maximum.Coordinates[1] >= _center.Coordinates[1])
                    return true;

                // the rect is in circle's second local quadrant
                var squares = GetSquares(@in);

                // bottom left
                return ((squares.LeftSquared + squares.BottomSquared) < _radiusSquared);
            }
            else
            {
                // fourth quadrant - top left
                var squares = GetSquares(@in);

                // bottom left
                return ((squares.LeftSquared + squares.TopSquared) < _radiusSquared);
            }
        }

        public override double MinimumDistanceFrom(IShape @in)
        {
            var box = @in.BoundingRegion;

            // is the box just not crossing between the min and max boundaries of the circles
            if (box.Minimum.Coordinates[0] <= _center.Coordinates[0])
            {
                // check if the rect x crosses over the peak of the circle
                if (box.Maximum.Coordinates[0] >= _center.Coordinates[0])
                {
                    // @in horizontally spans he circle's closest point
                    if (box.Maximum.Coordinates[1] <= (_center.Coordinates[1] - _radius))
                        return (_center.Coordinates[1] - _radius) - box.Maximum.Coordinates[1];
                    if (box.Minimum.Coordinates[1] >= (_center.Coordinates[1] + _radius))
                        return box.Minimum.Coordinates[1] - (_center.Coordinates[1] + _radius);
                    return 0.0;
                }

                if (box.Minimum.Coordinates[1] <= _center.Coordinates[1])
                {
                    // check if rect y crosses over the circle's peak
                    if (box.Maximum.Coordinates[1] >= _center.Coordinates[1])
                    {
                        // @in vertically spans the circle's closest point
                        if (box.Maximum.Coordinates[0] <= (_center.Coordinates[0] - _radius))
                            return (_center.Coordinates[0] - _radius) - box.Maximum.Coordinates[0];
                        if (box.Minimum.Coordinates[0] >= (_center.Coordinates[0] + _radius))
                            return box.Minimum.Coordinates[0] - (_center.Coordinates[0] + _radius);

                        // the circle intersects
                        return 0.0;
                    }

                    // the rect is in circle's first local quadrant
                    var squares = GetSquares(@in);

                    // check for intersection
                    var osquared = squares.RightSquared + squares.BottomSquared;
                    if (osquared <= _radiusSquared)
                        return 0.0;

                    // get the distance from the bottom to the circle
                    return Math.Sqrt(osquared - _radiusSquared);
                }
                else
                {
                    // the rect is in circle's third local quadrant
                    var squares = GetSquares(@in);

                    // top right
                    var osquared = squares.RightSquared + squares.TopSquared;
                    if (osquared <= _radiusSquared)
                        return 0.0;

                    return Math.Sqrt(osquared - _radiusSquared);
                }

            }

            if (box.Minimum.Coordinates[1] <= _center.Coordinates[1])
            {
                // still if we vertically cross the center, we're good
                if (box.Maximum.Coordinates[1] >= _center.Coordinates[1])
                {
                    // @in vertically spans the circle's closest point
                    if (box.Maximum.Coordinates[0] <= (_center.Coordinates[0] - _radius))
                        return (_center.Coordinates[0] - _radius) - box.Maximum.Coordinates[0];
                    if (box.Minimum.Coordinates[0] >= (_center.Coordinates[0] + _radius))
                        return box.Minimum.Coordinates[0] - (_center.Coordinates[0] + _radius);
                }

                // the rect is in circle's second local quadrant
                var squares = GetSquares(@in);

                // bottom left
                var osquared = squares.LeftSquared + squares.BottomSquared;
                if (osquared <= _radiusSquared)
                    return 0.0;

                return Math.Sqrt(osquared - _radiusSquared);
            }
            else
            {
                // fourth quadrant - top left
                var squares = GetSquares(@in);

                // bottom left
                var osquared = squares.LeftSquared + squares.TopSquared;
                if (osquared <= _radiusSquared)
                    return 0.0;

                return Math.Sqrt(osquared - _radiusSquared);
            }
        }

        public override bool Touches(IShape @in)
        {
            // this is the same thing as intersect but with <= instead of <
            var box = @in.BoundingRegion;

            // is the box just not crossing between the min and max boundaries of the circle?
            if (box.Maximum.Coordinates[0] < (_center.Coordinates[0] - _radius))
                return false;
            if (box.Minimum.Coordinates[0] > (_center.Coordinates[0] + _radius))
                return false;

            if (box.Maximum.Coordinates[1] < (_center.Coordinates[1] - _radius))
                return false;
            if (box.Minimum.Coordinates[1] > (_center.Coordinates[1] + _radius))
                return false;

            if (box.Minimum.Coordinates[0] <= _center.Coordinates[0])
            {
                if (box.Maximum.Coordinates[0] >= _center.Coordinates[0])
                    return true;

                if (box.Minimum.Coordinates[1] <= _center.Coordinates[1])
                {
                    if (box.Maximum.Coordinates[1] >= _center.Coordinates[1])
                        return true;

                    var squares = GetSquares(@in);
                    return ((squares.RightSquared + squares.BottomSquared) <= _radiusSquared);
                }
                else
                {
                    var squares = GetSquares(@in);
                    return ((squares.RightSquared + squares.TopSquared) <= _radiusSquared);
                }

            }

            if (box.Minimum.Coordinates[1] <= _center.Coordinates[1])
            {
                if (box.Maximum.Coordinates[1] >= _center.Coordinates[1])
                    return true;

                var squares = GetSquares(@in);
                return ((squares.LeftSquared + squares.BottomSquared) <= _radiusSquared);
            }
            else
            {
                var squares = GetSquares(@in);
                return ((squares.LeftSquared + squares.TopSquared) <= _radiusSquared);
            }
        }

        private CachedSquares GetSquares(IShape @in)
        {
            var innerRegion = @in.BoundingRegion;

            var topLeft = innerRegion.Minimum;
            var bottomRight = innerRegion.Maximum;

            // the inner region top left of the bounding rect
            var ntl = Tuple.Create(
                topLeft.Coordinates[0] - _center.Coordinates[0],
                topLeft.Coordinates[1] - _center.Coordinates[1]);

            // the inner region bottom right of the bounding rect
            var nbr = Tuple.Create(
                bottomRight.Coordinates[0] - _center.Coordinates[0],
                bottomRight.Coordinates[1] - _center.Coordinates[1]);

            // some cached math
            CachedSquares squares = new CachedSquares
            {
                TopSquared = ntl.Item2 * ntl.Item2,
                BottomSquared = nbr.Item2 * nbr.Item2,
                LeftSquared = ntl.Item1 * ntl.Item1,
                RightSquared = nbr.Item1 * nbr.Item1
            };
            return squares;
        }

        private struct CachedSquares
        {
            public double TopSquared;
            public double BottomSquared;
            public double LeftSquared;
            public double RightSquared;
        }

        private readonly Point _center;
        private readonly double _radius;
        private readonly double _radiusSquared;
    }
}
