using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PickyBrideProblem
{
    public interface IContenderGenerator
    {
        void Init(int AmountOfContenders);
        List<Contender> InitContenderList();           
    }
}
