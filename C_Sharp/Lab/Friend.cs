namespace PickyBrideProblem
{
    /// <summary>
    /// Friend compare two contenders, who already meet Princess
    /// </summary>
    public class Friend
    {
        private IHallForFriend _hall;

        public Friend(IHallForFriend hall)
        {
            _hall = hall;
        }

        public IContender Compare(IContender contender1, IContender contender2)
        {
            // if Princess didn't meet one of this contenders contender yet do not compare
            if (_hall.IsInHall(contender1.Name) || _hall.IsInHall(contender2.Name))
            {
                throw new System.Exception("Error: contenders cannot be compared!");
            }
            return _hall.GetMarkForFriend(contender1.Name) >= _hall.GetMarkForFriend(contender2.Name) ? contender1 : contender2;
        }
    }
}
