
using System;
using System.Collections.Generic;

namespace Konscious.SpatialIndex.Test.Implementations
{
    public class ManagedMemoryStorageManager : IManagedStorageManager
    {
        public ManagedMemoryStorageManager()
        {
            _data = new List<byte[]>();
            _freepages = new Queue<int>();
        }

        public override void Delete(long page)
        {
            if (page < 0 || page >= _data.Count)
            {
                return;
            }

            var pg = (int) page;
            _data[pg] = null;
            _freepages.Enqueue(pg);
        }

        public override void Flush()
        {
        }

        public override byte[] Load(long page)
        {
            if (page < 0 || page >= _data.Count)
            {
                throw new IndexOutOfRangeException();
            }
            
            var pg = (int) page;
            var fnd = _data[pg];

            if (fnd == null)
            {
                throw new IndexOutOfRangeException();
            }

            return fnd;
        }

        public override long Store(byte[] data)
        {
            if (_freepages.Count > 0)
            {
                var rtpage = _freepages.Dequeue();
                _data[rtpage] = data;
                return rtpage;
            }
            else
            {
                var rtpage = _data.Count;
                _data.Add(data);
                return rtpage;
            }
        }

        public override long Store(long suggestedPage, byte[] data)
        {
            if (suggestedPage < 0 || suggestedPage >= _data.Count)
            {
                return Store(data);
            }

            _data[(int)suggestedPage] = data;
            return suggestedPage;
        }

        private readonly List<byte[]> _data;
        private readonly Queue<int> _freepages;
    }
}
