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
            var boundingBox = @in.BoundingRegion;
            if (boundingBox.Minimum.Coordinates[0] <= _center.Coordinates[0])
            {
                // is the right most point of the shape left of the circle?
                if (boundingBox.Maximum.Coordinates[0] < (_center.Coordinates[0] - _radius))
                    return false;

                // @in is to the left of the circle's center but still horizontally crosses
                if (boundingBox.Minimum.Coordinates[1] <= _center.Coordinates[1])
                {
                    // see if the shape's bottom edge is still above the circle's top
                    if (boundingBox.Maximum.Coordinates[1] < (_center.Coordinates[1] - _radius))
                        return false;

                    // do we span accross the center of the circle, vertically?
                    if (boundingBox.Maximum.Coordinates[1] >= _center.Coordinates[1])
                        return true;

                    // we are crossing vertically - if the rectangle horizontally spans
                    // the circle, we still have a match
                    if (boundingBox.Maximum.Coordinates[0] >= _center.Coordinates[0])
                        return true;
                    
                    // last option: check the bottom right of the rectangle
                    var squares = GetSquares(@in);

                    // see if the bottom right is within the radius
                    if ((squares.RightSquared + squares.BottomSquared) > _radiusSquared)
                    {
                        return false;
                    }

                    return true;
                }

                // so here, the rectangle is still to the left of the circle's x=0 but the top is
                // in the bottom half somewhere

                // See if the rectangle is beyond the circle in general
                if (boundingBox.Minimum.Coordinates[1] > (_center.Coordinates[1] + _radius))
                {
                    return false;
                }

                // so if we horizontally span the circle's center, we'll still cross the long point
                if (boundingBox.Maximum.Coordinates[0] >= _center.Coordinates[0])
                    return true;

                // if not, we have a block to the bottom left of the circle's center.
                    
            }
            
            if (boundingBox.Minimum.Coordinates[0] > (_center.Coordinates[0] + _radius))
            {
                return false;
            }
        }

        public override double MinimumDistanceFrom(IShape @in)
        {
            throw new NotImplementedException();
        }

        public override bool Touches(IShape @in)
        {
            throw new NotImplementedException();
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
