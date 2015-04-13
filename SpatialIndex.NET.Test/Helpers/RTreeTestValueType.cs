using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Konscious.SpatialIndex.Test.Helpers
{
    [Serializable]
    public class RTreeTestValueType
    {
        public string Name { get; set; }

        public Guid Id { get; set; }

        public int Index { get; set; }

        public string Description { get; set; }
    }
}
