using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PickyBrideProblem
{
    public interface IHallForPrincess
    {
        int AmountOfContenders { get; }
        IContender InviteContender();
        int GetMarkForPrincess(string name);
        void Init();
    }
}