using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PickyBrideProblem
{
    public class ContenderGenerator : IContenderGenerator
    {
        List<Contender> _contenders = new List<Contender>();

        public void Init(int amountOfContenders)
        {
            if (amountOfContenders < 0 || amountOfContenders > 100)
                throw new Exception("Error: impossible to create list of less than 0 or more than 100 Contenders!");
            string line;
            StreamReader sr = new StreamReader("OneHundredUniqueNames.txt");
            for (int i = 1; i <= amountOfContenders; i++)
            {
                line = sr.ReadLine();
                Contender contender = new Contender(line, i);
                _contenders.Add(contender);
            }
            sr.Close();
        }

        public List<Contender> InitContenderList()
        {
            Random rnd = new Random();
            int minContendersMark = 1;
            int maxContendersMark = 100;
            var _contendersForHall = _contenders.OrderBy(x => rnd.Next(minContendersMark, maxContendersMark + 1));
            return (List<Contender>)_contendersForHall;
        }
    }
}
