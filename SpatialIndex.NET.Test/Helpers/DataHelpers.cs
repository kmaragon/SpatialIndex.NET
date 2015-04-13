using System;

namespace Konscious.SpatialIndex.Test.Helpers
{
    public static class DataHelpers
    {
        public static byte[] GenerateSomeBytes(int seed)
        {
            var gen = new Random(seed);
            var buffer = new byte[gen.Next(10, 1000)];
            gen.NextBytes(buffer);

            return buffer;
        }
    }
}
