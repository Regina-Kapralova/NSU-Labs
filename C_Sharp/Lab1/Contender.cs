
namespace PickyBrideProblem
{
    /// <summary>
    /// Contender is man who can become Princess's husband. Contender has unique name and unique mark from 1 to 100. 
    /// </summary>
    public class Contender : IContender
    {
        public string Name { get; private set; }
        public int Mark { get; private set; }

        public Contender(string name, int mark)
        {
            Name = name;
            Mark = mark;
        }
    }
}
