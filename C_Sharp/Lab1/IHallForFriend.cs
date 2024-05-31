using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PickyBrideProblem
{
    public interface IHallForFriend
    {
        bool IsInHall(string name);
        int GetMarkForFriend(string name);
    }
}
