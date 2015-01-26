using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpatialIndex.NET.Test.Helpers
{
    public static class DataHelpers
    {
        public static byte[] GenerateSomeBytes(int seed)
        {
            var gen = new Random(seed);
            var buffer = new byte[gen.Next(100, 100000)];
            gen.NextBytes(buffer);

            return buffer;
        }
    }
}
